@echo off

echo.
echo �Ǘ��Ҍ����� FlushMouse �̋N�����^�X�N�X�P�W���[���[�֓o�^���܂�
echo.
timeout /T 3

for /f "tokens=3 delims=\ " %%A in ('whoami /groups^|find "Mandatory"') do set LEVEL=%%A
if not "%LEVEL%"=="High" (
powershell -NoProfile -ExecutionPolicy Unrestricted -Command "Start-Process \"%~f0\" -Verb runas"
exit
)

schtasks.exe /Create /F /TN "FlushMouse" /XML "C:\Program Files\JeffyTS\FlushMouse\FlushMouse.xml"

echo.
echo �I�����܂���
timeout /T 3
exit

