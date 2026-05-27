@echo off
setlocal enabledelayedexpansion

:: --- [ตั้งค่าส่วนนี้] ---
set "PY_SCRIPT=myscript.py"
set "REQ_FILE=requirements.txt"
set "PY_VER=3.12.3"
set "PYTHON_URL=https://www.python.org/ftp/python/%PY_VER%/python-%PY_VER%-amd64.exe"
set "INSTALLER=python_setup.exe"
:: ----------------------

echo [Step 1] Checking Python Installation...
python --version >nul 2>&1
if %errorlevel% neq 0 (
    echo Python not found. Downloading and Installing...
    curl -L "%PYTHON_URL%" --output %INSTALLER%
    start /wait %INSTALLER% /quiet InstallAllUsers=1 PrependPath=1
    del %INSTALLER%
    :: Refresh path สำหรับ Session นี้
    set "PATH=%PATH%;%ProgramFiles%\Python312\;%ProgramFiles%\Python312\Scripts\"
) else (
    echo Python is already installed.
)

echo [Step 2] Checking and Installing Libraries...
if exist "%REQ_FILE%" (
    echo Found %REQ_FILE%. Updating libraries...
    :: -m pip install -r จะเช็คเองว่าตัวไหนมีแล้วจะข้าม ตัวไหนไม่มีจะโหลดเพิ่ม
    python -m pip install --upgrade pip
    python -m pip install -r "%REQ_FILE%"
) else (
    echo [Skip] %REQ_FILE% not found. No libraries to install.
)

echo [Step 3] Running Script...
if exist "%PY_SCRIPT%" (
    echo ---------------------------------------
    echo Running: %PY_SCRIPT%
    echo ---------------------------------------
    python "%PY_SCRIPT%"
    echo ---------------------------------------
) else (
    echo Error: Python script "%PY_SCRIPT%" not found.
)

pause