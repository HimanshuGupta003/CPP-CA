@echo off
setlocal
cd /d "%~dp0"
if not exist "build" mkdir "build"
g++ -std=c++11 -Wall -Wextra -pedantic "src\52_CAP455_CA2_Community_Newspaper_Editorial_Desk.cpp" -o "build\newspaper_ca2.exe"
if errorlevel 1 (
    echo Compilation failed. Ensure MinGW g++ is in your PATH.
    pause
    exit /b 1
)
"build\newspaper_ca2.exe"
pause

