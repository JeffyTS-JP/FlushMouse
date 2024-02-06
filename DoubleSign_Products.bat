@echo off

if NOT EXIST "%USERPROFILE%\Program Files\SignTool\DoubleSign_Products.bat" goto DONE

copy /Y SetupProject-exe\bin\Release\FlushMouse_x64.exe SetupProject-exe\bin\Release\FlushMouse_x64_signed.exe >NUL
copy /Y FlushMouseUI3\SetupProjectUI3-exe\bin\Release\FlushMouseUI3_x64.exe FlushMouseUI3\SetupProjectUI3-exe\bin\Release\FlushMouseUI3_x64_signed.exe >NUL

echo.
echo ===== Desktop
CALL "%USERPROFILE%\Program Files\SignTool\DoubleSign_Products.bat" SetupProject-exe\bin\Release\FlushMouse_x64_signed.exe

echo.
echo ===== UI3
CALL "%USERPROFILE%\Program Files\SignTool\DoubleSign_Products.bat" FlushMouseUI3\SetupProjectUI3-exe\bin\Release\FlushMouseUI3_x64_signed.exe

:DONE
exit
