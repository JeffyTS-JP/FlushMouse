@echo off

echo.
echo 管理者権限で FlushMouse の起動をタスクスケジューラーから削除します
echo.
timeout /T 3

for /f "tokens=3 delims=\ " %%A in ('whoami /groups^|find "Mandatory"') do set LEVEL=%%A
if not "%LEVEL%"=="High" (
powershell -NoProfile -ExecutionPolicy Unrestricted -Command "Start-Process \"%~f0\" -Verb runas"
exit
)

schtasks.exe /Delete /F /TN "FlushMouse"

echo.
echo 終了しました
timeout /T 3
exit

