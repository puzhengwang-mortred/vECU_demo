@echo off
echo Generating Visual Studio Solution for vECU Demo...

REM Create build directory if it doesn't exist
if not exist "build" mkdir build

REM Navigate to build directory
cd build

REM Generate Visual Studio solution (this assumes Visual Studio 2019 or newer)
cmake .. -G "Visual Studio 16 2019" -A x64

REM Check if cmake command was successful
if %errorlevel% neq 0 (
    echo CMake generation failed!
    echo Make sure you have:
    echo 1. SilKit properly installed and SilKit_DIR environment variable set
    echo 2. Visual Studio with C++ development tools installed
    echo 3. CMake installed and in your PATH
    pause
    exit /b %errorlevel%
)

echo.
echo Visual Studio solution generated successfully!
echo.
echo To open the solution:
echo 1. Open build\vECU_Demo.sln in Visual Studio
echo 2. Or run: start vECU_Demo.sln
echo.
echo To build from command line:
echo cmake --build . --config Release
echo.
pause