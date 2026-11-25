@echo off
setlocal
cd /d "%~dp0"
set BTB_MAX_FPS=240
set BTB_PRESENT_MODE=fifo
set BTB_CONTINUOUS_PRESENT=0
set BTB_HARDWARE_CURSOR=1
set BTB_GPU_SPRITES=1
set BTB_GPU_BINK=1
set BTB_GPU_LAYERS=1
set BTB_INTERPOLATE_SPRITES=0
set BTB_SHOW_FPS=1
set BTB_PROFILE_DIR=%LOCALAPPDATA%\BTB-Enhanced\profiles
"%~dp0btb_game_enhanced.exe" %*
if errorlevel 1 pause
