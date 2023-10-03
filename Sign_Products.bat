@echo off

if NOT EXIST "%USERPROFILE%\Program Files\SignTool\Sign_Products.bat" goto DONE

"%USERPROFILE%\Program Files\SignTool\Sign_Products.bat" %*

:DONE
exit