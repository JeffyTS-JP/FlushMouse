@echo off

if NOT EXIST "%USERPROFILE%\Program Files\SignTool\Sign_Modules.bat" goto DONE

"%USERPROFILE%\Program Files\SignTool\Sign_Modules.bat" %*

:DONE
exit