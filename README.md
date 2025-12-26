# vECU Demo with SilKit CAN Communication

This project demonstrates a virtual ECU setup using SilKit for CAN communication between two Simulink-generated models:

- **SpeedController**: An Adaptive Speed Controller model that receives vehicle speed and outputs brake/throttle commands
- **VehicleSpeedModel**: A plant model that simulates vehicle dynamics based on brake/throttle inputs

## Architecture

The two models communicate via CAN bus using SilKit:
- SpeedController ECU receives vehicle speed via CAN ID 0x100 and sends control commands via CAN ID 0x200
- VehicleSpeedModel ECU receives control commands via CAN ID 0x200 and sends vehicle speed via CAN ID 0x100

## Build Instructions

### Using CMake (Command Line)
1. Make sure you have SilKit installed and the SilKit_DIR environment variable set
2. Create a build directory and run CMake:

```bash
mkdir build
cd build
cmake ..
cmake --build .
```

### Using Visual Studio
1. Make sure you have SilKit installed and the SilKit_DIR environment variable set
2. Run the `generate_vs_solution_advanced.bat` script to generate the Visual Studio solution
3. Open the generated `build/vECU_Demo.sln` in Visual Studio
4. Build the solution in Visual Studio

## Running the Simulation

1. Start the SilKit Registry:
   ```bash
   SilKitRegistry --registry-uri silkit://localhost:8500
   ```

2. Run both participants in separate terminals:
   ```bash
   # Terminal 1
   ./build/bin/VehicleSpeedModelECU VehicleSpeedModelECU
   
   # Terminal 2
   ./build/bin/SpeedControllerECU SpeedControllerECU
   ```

3. The two participants will establish CAN communication and start exchanging data.

## File Structure

- `SpeedController_ert_rtw/`: Contains the SpeedController model generated code and `vECU_Main.cpp`
- `VehicleSpeedModel_grt_rtw/`: Contains the VehicleSpeedModel generated code and `vECU_PlantModel.cpp`
- `CMakeLists.txt`: Build configuration for both executables