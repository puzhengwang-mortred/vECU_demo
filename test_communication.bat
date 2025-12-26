@echo off
echo Starting SilKit Registry...
start /min SilKitRegistry --registry-uri silkit://localhost:8500

timeout /t 2 /nobreak >nul

echo Starting VehicleSpeedModel ECU participant...
start /min cmd /c "cd /d c:\working\_wpz\Projects\vECU\demo\build\bin && VehicleSpeedModelECU.exe VehicleSpeedModelECU"

timeout /t 2 /nobreak >nul

echo Starting SpeedController ECU participant...
start /min cmd /c "cd /d c:\working\_wpz\Projects\vECU\demo\build\bin && SpeedControllerECU.exe SpeedControllerECU"

echo Both participants started. Check the console windows for output.
echo To stop the simulation, close the console windows or end the processes manually.
pause