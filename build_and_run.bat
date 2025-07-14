@echo off
echo Building Flappy Bird MVP...
cd /d "%~dp0"
cd build
ninja
if %errorlevel% neq 0 (
    echo Build failed!
    pause
    exit /b 1
)
echo Build successful!
echo Opening game in browser...
start index.html
pause
