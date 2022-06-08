@echo off

echo.
echo 管理者権限で FlushMouse の起動をタスクスケジューラーへ登録します
echo.
timeout /T 3

for /f "tokens=3 delims=\ " %%A in ('whoami /groups^|find "Mandatory"') do set LEVEL=%%A
if not "%LEVEL%"=="High" (
powershell -NoProfile -ExecutionPolicy Unrestricted -Command "Start-Process \"%~f0\" -Verb runas"
exit
)

schtasks.exe /Create /F /TN "FlushMouse" /XML "C:\Program Files\JeffyTS\FlushMouse\FlushMouse.xml"

echo.
echo 終了しました
timeout /T 3
exit

