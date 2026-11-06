@echo off
setlocal
cd /d "%~dp0"
if not exist "build" mkdir "build"
g++ -std=c++11 -Wall -Wextra -pedantic "src\52_CAP455_CA1_Community_Newspaper_Editorial_Desk.cpp" -o "build\newspaper_ca1.exe"
if errorlevel 1 (
    echo Compilation failed. Make sure g++ is installed and available in PATH.
    pause
    exit /b 1
)
"build\newspaper_ca1.exe"
pause
