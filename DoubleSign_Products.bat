@echo off

if NOT EXIST "%USERPROFILE%\Program Files\SignTool\DoubleSign_Products.bat" goto DONE

copy /Y SetupProject-exe\bin\Release\FlushMouse_x64.exe .\FlushMouse_x64_signed.exe >NUL
"%USERPROFILE%\Program Files\SignTool\DoubleSign_Products.bat" FlushMouse_x64_signed.exe

:DONE
exit