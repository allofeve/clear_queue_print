@echo off
setlocal enabledelayedexpansion

:: --- ตั้งค่าส่วนนี้ ---
set "PY_SCRIPT=myscript.py"
set "PY_VER=3.12.3"
set "PYTHON_URL=https://www.python.org/ftp/python/%PY_VER%/python-%PY_VER%-amd64.exe"
set "INSTALLER=python_setup.exe"
:: ------------------

echo [Checking System]
python --version >nul 2>&1

if %errorlevel% == 0 (
    echo Python is ready.
    goto :RUN_PYTHON
) else (
    echo Python not found. Starting Installation...
    goto :INSTALL_PYTHON
)

:INSTALL_PYTHON
echo [Downloading] Python %PY_VER%...
curl -L "%PYTHON_URL%" --output %INSTALLER%

if not exist %INSTALLER% (
    echo Error: Download failed.
    pause
    exit /b
)

echo [Installing] Please wait...
start /wait %INSTALLER% /quiet InstallAllUsers=1 PrependPath=1 Include_test=0

echo [Cleaning up] Removing installer...
del %INSTALLER%

:: รีเฟรช Environment Variables เฉพาะหน้าต่างนี้เพื่อให้ใช้คำสั่ง python ได้ทันที
set "PATH=%PATH%;%ProgramFiles%\Python312\;%ProgramFiles%\Python312\Scripts\"

echo Installation finished.
goto :RUN_PYTHON

:RUN_PYTHON
if exist "%PY_SCRIPT%" (
    echo [Running] %PY_SCRIPT%...
    echo ---------------------------------------
    python "%PY_SCRIPT%"
    echo ---------------------------------------
) else (
    echo Error: Python script "%PY_SCRIPT%" not found in this folder.
)

pause