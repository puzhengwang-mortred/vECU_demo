#include "VehicleSpeedModel.h"
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

// VehicleSpeedModel wrapper class as plant model with CAN communication
class VehicleSpeedModelASW {
private:
    SilKit::IParticipant* _participant;
    SilKit::Services::Can::ICanController* _canController;
    
public:
    VehicleSpeedModelASW(SilKit::IParticipant* participant) : _participant(participant) {
        // Initialize VehicleSpeedModel
        VehicleSpeedModel_initialize();
        
        // Setup CAN communication
        setupCanCommunication();
    }
    
    ~VehicleSpeedModelASW() {
        VehicleSpeedModel_terminate();
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
        
        std::cout << "VehicleSpeedModelASW: CAN communication setup complete" << std::endl;
    }
    
    void onCanFrameReceived(const SilKit::Services::Can::CanFrameEvent& frameEvent) {
        // Handle different types of CAN frames based on CAN ID
        if (frameEvent.frame.canId == 0x200) {  // Control command message ID
            if (frameEvent.frame.dataField.size() >= sizeof(ControlCommandData)) {
                // Extract control commands from received CAN frame
                ControlCommandData receivedData;
                std::memcpy(&receivedData, frameEvent.frame.dataField.data(), sizeof(ControlCommandData));
                
                // Set inputs to VehicleSpeedModel
                VehicleSpeedModel_U.Throttle_Input = receivedData.throttle;
                VehicleSpeedModel_U.Brake_Input = receivedData.brake;
                
                // Execute VehicleSpeedModel step
                VehicleSpeedModel_step();
                
                // Get output from VehicleSpeedModel
                double speedOutput = VehicleSpeedModel_Y.Speed_Output;
                
                // Send speed data via CAN
                sendVehicleSpeed(speedOutput);
                
                std::cout << "VehicleSpeedModelASW: Processed throttle=" << receivedData.throttle 
                          << ", brake=" << receivedData.brake << ", speed=" << speedOutput << std::endl;
            }
        }
    }
    
    void sendVehicleSpeed(double speed) {
        VehicleSpeedData speedData;
        speedData.speed = speed;
        
        SilKit::Services::Can::CanFrame frame;
        frame.canId = 0x100;  // Vehicle speed message ID
        frame.dlc = sizeof(VehicleSpeedData);
        frame.dataField.resize(sizeof(VehicleSpeedData));
        std::memcpy(frame.dataField.data(), &speedData, sizeof(VehicleSpeedData));
        
        _canController->SendFrame(frame);
    }
    
    // Method to get current speed output from the model
    double getCurrentSpeed() {
        return VehicleSpeedModel_Y.Speed_Output;
    }
};

int main(int argc, char* argv[]) {
    try {
        std::string participantName = "VehicleSpeedModelECU";
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
        
        // Register the VehicleSpeedModel as an ASW component
        VehicleSpeedModelASW vehicleSpeedModelASW(participant.get());
        
        // Get orchestration service to manage lifecycle
        auto* lifecycleService = participant->CreateLifecycleService({SilKit::Services::Orchestration::OperationMode::Coordinated});
        
        // Set up lifecycle handlers
        lifecycleService->SetCommunicationReadyHandler([&vehicleSpeedModelASW]() {
            std::cout << "VehicleSpeedModel: Communication ready" << std::endl;
            // Send initial speed to start the closed loop
            vehicleSpeedModelASW.sendVehicleSpeed(0.0);
        });
        
        lifecycleService->SetStopHandler([]() {
            std::cout << "VehicleSpeedModel: Stop requested" << std::endl;
        });
        
        lifecycleService->SetShutdownHandler([]() {
            std::cout << "VehicleSpeedModel: Shutting down" << std::endl;
        });
        
        std::cout << "VehicleSpeedModel virtual ECU started. Waiting for control commands..." << std::endl;
        
        // Start the lifecycle - this will block until the simulation ends
        auto future = lifecycleService->StartLifecycle();
        auto finalState = future.get();
        
        std::cout << "VehicleSpeedModel final state: " << finalState << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in VehicleSpeedModel virtual ECU: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}