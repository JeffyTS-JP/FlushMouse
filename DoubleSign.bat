@echo off

if NOT EXIST "%USERPROFILE%\Program Files\SignTool\DoubleSign_Products.bat" goto DONE

echo.
echo.
echo ===================
echo       Desktop
echo ===================
copy /Y SetupProject-exe\bin\Release\FlushMouse_x64.exe FlushMouse_x64_signed.exe >NUL
CALL "%USERPROFILE%\Program Files\SignTool\DoubleSign_Products.bat" FlushMouse_x64_signed.exe

echo.
echo.
echo ===================
echo         UI3
echo ===================
copy /Y FlushMouseUI3\SetupProjectUI3-exe\bin\Release\FlushMouseUI3_x64.exe FlushMouseUI3_x64_signed.exe >NUL
CALL "%USERPROFILE%\Program Files\SignTool\DoubleSign_Products.bat" FlushMouseUI3_x64_signed.exe

if NOT EXIST "%USERPROFILE%\Program Files\SignTool\DoubleSign_Modules.bat" goto DONE

echo.
echo.
echo ===================
echo   SynTP_Receiver
echo ===================
copy /Y SynTP_Receiver.exe SynTP_Receiver_signed.exe >NUL
CALL "%USERPROFILE%\Program Files\SignTool\DoubleSign_Modules.bat" SynTP_Receiver_signed.exe

if NOT EXIST "SynTP_Receiver_signed.exe" goto DONE
copy /Y SynTP_Receiver_signed.exe SynTP_Receiver.exe
del /Q SynTP_Receiver_signed.exe

echo.
echo =========================
echo  Make SynTP_Receiver.zip
echo =========================
if EXIST "SynTP_Receiver.zip" del /Q "SynTP_Receiver.zip"
if EXIST "%TEMP%\SynTP_Receiver" rmdir /S /Q "%TEMP%\SynTP_Receiver"
mkdir "%TEMP%\SynTP_Receiver"
copy /Y "SynTP_Receiver.exe" "%TEMP%\SynTP_Receiver"
copy /Y "SetupProject\Readme.txt" "%TEMP%\SynTP_Receiver"
zip -rj SynTP_Receiver.zip "%TEMP%\SynTP_Receiver"
rmdir /S /Q "%TEMP%\SynTP_Receiver"


echo.
echo èIóπÇµÇ‹ÇµÇΩ
echo.
pause

:DONE
exit
