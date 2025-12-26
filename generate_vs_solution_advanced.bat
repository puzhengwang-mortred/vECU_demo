@echo off
echo Detecting available Visual Studio versions...

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Navigate to build directory
cd build

echo Attempting to generate Visual Studio solution...

REM Try different Visual Studio versions in order of preference
cmake .. -G "Visual Studio 17 2022" -A x64 2>nul
if %errorlevel% equ 0 (
    echo Successfully generated solution for Visual Studio 2022
    goto success
)

cmake .. -G "Visual Studio 16 2019" -A x64 2>nul
if %errorlevel% equ 0 (
    echo Successfully generated solution for Visual Studio 2019
    goto success
)

cmake .. -G "Visual Studio 15 2017" -A x64 2>nul
if %errorlevel% equ 0 (
    echo Successfully generated solution for Visual Studio 2017
    goto success
)

REM If none of the above worked, try with the default generator
cmake .. -A x64
if %errorlevel% neq 0 (
    echo.
    echo CMake generation failed!
    echo.
    echo Make sure you have:
    echo 1. SilKit properly installed and SilKit_DIR environment variable set
    echo 2. Visual Studio with C++ development tools installed
    echo 3. CMake installed and in your PATH
    echo.
    echo You can also try manually specifying the generator:
    echo cmake .. -G "Visual Studio XX YYYY" -A x64
    echo.
    pause
    exit /b %errorlevel%
)

echo Successfully generated solution with default generator

:success
echo.
echo Visual Studio solution generated successfully!
echo.
echo To open the solution:
echo 1. Open build\vECU_Demo.sln in Visual Studio
echo 2. Or run: start vECU_Demo.sln from the build directory
echo.
echo To build from command line:
echo cmake --build . --config Release
echo.
echo To run the applications after building:
echo 1. Start SilKit Registry: SilKitRegistry --registry-uri silkit://localhost:8500
echo 2. Run VehicleSpeedModel: .\bin\VehicleSpeedModelECU.exe VehicleSpeedModelECU
echo 3. Run SpeedController: .\bin\SpeedControllerECU.exe SpeedControllerECU
echo.
pause