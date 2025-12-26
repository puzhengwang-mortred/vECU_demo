Write-Host "Detecting available Visual Studio versions..." -ForegroundColor Green

# Create build directory if it doesn't exist
if (!(Test-Path "build")) {
    New-Item -ItemType Directory -Name "build" | Out-Null
}

# Navigate to build directory
Set-Location build

Write-Host "Attempting to generate Visual Studio solution..." -ForegroundColor Green

# Try different Visual Studio versions in order of preference
$generators = @(
    "Visual Studio 17 2022",
    "Visual Studio 16 2019", 
    "Visual Studio 15 2017"
)

$success = $false
foreach ($generator in $generators) {
    Write-Host "Trying generator: $generator" -ForegroundColor Yellow
    try {
        $result = cmake .. -G $generator -A x64 2>&1
        if ($LASTEXITCODE -eq 0) {
            Write-Host "Successfully generated solution for $generator" -ForegroundColor Green
            $success = $true
            break
        }
    }
    catch {
        # Continue to next generator
    }
}

if (-not $success) {
    Write-Host "Trying default generator..." -ForegroundColor Yellow
    cmake .. -A x64
    if ($LASTEXITCODE -ne 0) {
        Write-Host "CMake generation failed!" -ForegroundColor Red
        Write-Host ""
        Write-Host "Make sure you have:" -ForegroundColor Red
        Write-Host "1. SilKit properly installed and SilKit_DIR environment variable set" -ForegroundColor Red
        Write-Host "2. Visual Studio with C++ development tools installed" -ForegroundColor Red
        Write-Host "3. CMake installed and in your PATH" -ForegroundColor Red
        Write-Host ""
        Write-Host "You can also try manually specifying the generator:" -ForegroundColor Yellow
        Write-Host "cmake .. -G `"Visual Studio XX YYYY`" -A x64" -ForegroundColor Yellow
        Read-Host "Press Enter to continue"
        exit $LASTEXITCODE
    }
    Write-Host "Successfully generated solution with default generator" -ForegroundColor Green
}

Write-Host ""
Write-Host "Visual Studio solution generated successfully!" -ForegroundColor Green
Write-Host ""
Write-Host "To open the solution:" -ForegroundColor White
Write-Host "1. Open build\vECU_Demo.sln in Visual Studio" -ForegroundColor White
Write-Host "2. Or run: start vECU_Demo.sln from the build directory" -ForegroundColor White
Write-Host ""
Write-Host "To build from command line:" -ForegroundColor White
Write-Host "cmake --build . --config Release" -ForegroundColor White
Write-Host ""
Write-Host "To run the applications after building:" -ForegroundColor White
Write-Host "1. Start SilKit Registry: SilKitRegistry --registry-uri silkit://localhost:8500" -ForegroundColor White
Write-Host "2. Run VehicleSpeedModel: .\bin\VehicleSpeedModelECU.exe VehicleSpeedModelECU" -ForegroundColor White
Write-Host "3. Run SpeedController: .\bin\SpeedControllerECU.exe SpeedControllerECU" -ForegroundColor White
Write-Host ""

Read-Host "Press Enter to continue"