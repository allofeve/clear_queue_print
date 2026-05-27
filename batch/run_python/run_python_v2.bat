@echo off
setlocal enabledelayedexpansion

:: ตั้งค่าเวอร์ชันและ URL (ตัวอย่างนี้ใช้ 3.12.3 สามารถปรับเปลี่ยนได้)
set "PY_VER=3.12.3"
set "PYTHON_URL=https://www.python.org/ftp/python/%PY_VER%/python-%PY_VER%-amd64.exe"
set "INSTALLER=python_setup.exe"

echo [1/3] Checking for Python installation...

:: ตรวจสอบว่ามีคำสั่ง python ในระบบหรือไม่
python --version >nul 2>&1
if %errorlevel% == 0 (
    echo Python is already installed.
    python --version
    goto :FINISH
) else (
    echo Python not found. Proceeding to download...
)

echo [2/3] Downloading Python %PY_VER%...
:: ใช้ curl ดาวน์โหลดไฟล์จากลิงก์โดยตรง (Direct Link)
curl -L "%PYTHON_URL%" --output %INSTALLER%

if not exist %INSTALLER% (
    echo Error: Failed to download Python installer.
    pause
    exit /b
)

echo [3/3] Installing Python (Silent Mode)...
echo Please wait, this may take a minute...
:: /quiet: ติดตั้งแบบไม่แสดงผลหน้าจอเลย
:: InstallAllUsers=1: ติดตั้งให้ทุก User ในเครื่อง
:: PrependPath=1: สำคัญมาก! เพื่อให้รันคำสั่ง python ได้ทันทีหลังติดตั้ง
start /wait %INSTALLER% /quiet InstallAllUsers=1 PrependPath=1 Include_test=0

echo Cleaning up...
del %INSTALLER%
echo.
echo Python installation finished successfully.
echo Note: You might need to restart your terminal to use 'python' command.

:FINISH
echo.
echo Operation completed.
pause