@echo off

echo.
echo �Ǘ��Ҍ����� FlushMouse �̋N�����^�X�N�X�P�W���[���[����폜���܂�
echo.
timeout /T 3

for /f "tokens=3 delims=\ " %%A in ('whoami /groups^|find "Mandatory"') do set LEVEL=%%A
if not "%LEVEL%"=="High" (
powershell -NoProfile -ExecutionPolicy Unrestricted -Command "Start-Process \"%~f0\" -Verb runas"
exit
)

schtasks.exe /Delete /F /TN "FlushMouse"

echo.
echo �I�����܂���
timeout /T 3
exit

