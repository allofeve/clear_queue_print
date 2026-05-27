@echo off
chcp 65001 >nul
:: ตรวจสอบและบังคับให้ Run as Administrator
REM  -----------------------------------------
VBCREATEFOLDER >nul 2>&1
net session >nul 2>&1
if %errorLevel% == 0 (
    goto :admin
) else (
    echo Requesting administrative privileges...
    goto :UACPrompt
)

:UACPrompt
    echo Set UAC = CreateObject^("Shell.Application"^) > "%temp%\getadmin.vbs"
    echo UAC.ShellExecute "cmd.exe", "/c """"%~s0""""", "", "runas", 1 >> "%temp%\getadmin.vbs"
    "%temp%\getadmin.vbs"
    del "%temp%\getadmin.vbs"
    exit /B

:admin
    pushd "%cd%"
    CD /D "%~dp0"
REM  -----------------------------------------

:: เริ่มต้นคำสั่งลบคิวปริ้นเตอร์
net stop spooler
del /Q /F /S "%systemroot%\System32\spool\Printers\*.*"
net start spooler

echo.
echo === ลบคิวปริ้นเตอร์ที่ค้างเรียบร้อยแล้ว ===
pause