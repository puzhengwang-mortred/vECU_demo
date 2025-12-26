#include "SpeedController.h"
#include <SilKit/SilKit.hpp>
#include <SilKit/Config.hpp>
#include <SilKit/Participant.hpp>
#include <SilKit/Services/Can.hpp>
#include <SilKit/Services/Datatypes.hpp>
#include <SilKit/Services/Orchestration.hpp>
#include <SilKit/Services/Logging.hpp>
#include <iostream>
#include <memory>
#include <thread>
#include <chrono>
#include <cstring>

// Data structures for CAN communication
struct VehicleSpeedData {
    double speed = 0.0;
};

struct ControlCommandData {
    double brake = 0.0;
    double throttle = 0.0;
};

// SpeedController wrapper class as ASW component with CAN communication
class SpeedControllerASW {
private:
    SilKit::IParticipant* _participant;
    SilKit::Services::Can::ICanController* _canController;
    
public:
    SpeedControllerASW(SilKit::IParticipant* participant) : _participant(participant) {
        // Initialize SpeedController
        SpeedController_initialize();
        
        // Setup CAN communication
        setupCanCommunication();
    }
    
    ~SpeedControllerASW() {
        SpeedController_terminate();
    }
    
    void setupCanCommunication() {
        // Create CAN controller
        _canController = _participant->CreateCanController("CAN1", "VehicleNetwork");
        
        // Add frame handler to receive CAN messages
        _canController->AddFrameHandler(
            [this](SilKit::Services::Can::ICanController* /*controller*/, 
                   const SilKit::Services::Can::CanFrameEvent& frameEvent) {
                this->onCanFrameReceived(frameEvent);
            });
        
        // Configure and start CAN controller
        _canController->SetBaudRate(500000);  // 500 kbps
        _canController->Start();
        
        std::cout << "SpeedControllerASW: CAN communication setup complete" << std::endl;
    }
    
    void onCanFrameReceived(const SilKit::Services::Can::CanFrameEvent& frameEvent) {
        // Handle different types of CAN frames based on CAN ID
        if (frameEvent.frame.canId == 0x100) {  // Vehicle speed message ID
            if (frameEvent.frame.dataField.size() >= sizeof(VehicleSpeedData)) {
                // Extract speed from received CAN frame
                VehicleSpeedData receivedData;
                std::memcpy(&receivedData, frameEvent.frame.dataField.data(), sizeof(VehicleSpeedData));
                
                // Set input to SpeedController
                SpeedController_U.Speed_Input = receivedData.speed;
                
                // Execute SpeedController step
                SpeedController_step();
                
                // Get outputs from SpeedController
                double brakeOutput = SpeedController_Y.Brake_Output;
                double throttleOutput = SpeedController_Y.Throttle_Output;
                
                // Prepare control command data
                ControlCommandData controlData;
                controlData.brake = brakeOutput;
                controlData.throttle = throttleOutput;
                
                // Send control commands via CAN
                sendControlCommands(controlData);
                
                std::cout << "SpeedControllerASW: Processed speed=" << receivedData.speed 
                          << ", brake=" << brakeOutput << ", throttle=" << throttleOutput << std::endl;
            }
        }
    }
    
    void sendControlCommands(const ControlCommandData& data) {
        // Prepare CAN frame for control commands
        SilKit::Services::Can::CanFrame frame;
        frame.canId = 0x200;  // Control command message ID
        frame.dlc = sizeof(ControlCommandData);
        
        // Copy control data to frame data field
        frame.dataField.resize(sizeof(ControlCommandData));
        std::memcpy(frame.dataField.data(), &data, sizeof(ControlCommandData));
        
        // Send the frame via CAN
        _canController->SendFrame(frame);
    }
};

int main(int argc, char* argv[]) {
    try {
        std::string participantName = "SpeedControllerECU";
        std::string configPath = ""; // Use default configuration or specify path
        
        // Parse command line arguments
        if (argc > 1) {
            participantName = argv[1];
        }
        if (argc > 2) {
            configPath = argv[2];
        }
        
        // Create SIL Kit participant
        auto participant = SilKit::CreateParticipant(SilKit::Config::FromString(configPath), participantName);
        
        // Register the SpeedController as an ASW component
        SpeedControllerASW speedControllerASW(participant.get());
        
        // Get orchestration service to manage lifecycle
        auto* lifecycleService = participant->CreateLifecycleService({SilKit::Services::Orchestration::OperationMode::Coordinated});
        
        // Set up lifecycle handlers
        lifecycleService->SetCommunicationReadyHandler([]() {
            std::cout << "SpeedController: Communication ready" << std::endl;
        });
        
        lifecycleService->SetStopHandler([]() {
            std::cout << "SpeedController: Stop requested" << std::endl;
        });
        
        lifecycleService->SetShutdownHandler([]() {
            std::cout << "SpeedController: Shutting down" << std::endl;
        });
        
        std::cout << "SpeedController virtual ECU started. Waiting for vehicle speed data..." << std::endl;
        
        // Start the lifecycle - this will block until the simulation ends
        auto future = lifecycleService->StartLifecycle();
        auto finalState = future.get();
        
        std::cout << "SpeedController final state: " << finalState << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in SpeedController virtual ECU: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}

