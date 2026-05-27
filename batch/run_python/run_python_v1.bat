@echo off
:: Set version and download link
set "PYTHON_URL=https://python.org"
set "INSTALLER=python_setup.exe"

echo Downloading Python installer...
curl -L "%PYTHON_URL%" --output %INSTALLER%

echo Installing Python (Silent Mode)...
:: /passive: Show progress bar but no interaction
:: PrependPath=1: Add Python to Windows environment variables
%INSTALLER% /passive InstallAllUsers=1 PrependPath=1 Include_test=0

echo Cleaning up...
del %INSTALLER%
echo Python installation finished.
pause
