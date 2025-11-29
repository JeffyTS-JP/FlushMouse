rem @echo off

rem ============================
rem Unregister Registries & Files
rem		Copyright (C) 1993- JeffyTS. All rights reserved.
rem		Licensed under the MIT License.
rem Unregister.bat PRODUCTED_ID BUNDLED_ID
rem ============================

set MANUFACTURER=JeffyTS
set PRODUCTNAME=FlushMouse
set REG_EXEC="%SystemRoot%\System32\reg.exe"

rem %REG_EXEC% delete HKLM\SOFTWARE\WOW6432Node\%MANUFACTURER% /F
rem %REG_EXEC% delete HKLM\SOFTWARE\%MANUFACTURER%\%PRODUCTNAME%\InstalledPath /F
rem %REG_EXEC% delete HKLM\SOFTWARE\%MANUFACTURER%\%PRODUCTNAME%\DataInstalledPath /F

@if "%~1"=="" (
echo "引数なし"
exit
) else (
rem %REG_EXEC% delete HKLM\SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\%~1 /F
rem %REG_EXEC% delete HKLM\SOFTWARE\Installer\Dependencies\%~1 /F
%REG_EXEC% delete HKLM\SOFTWARE\Classes\Installer\Dependencies\%~1 /F
)

@if "%~2"=="" (
echo "引数なし"
exit
) else (
%REG_EXEC% delete HKLM\SOFTWARE\WOW6432Node\Microsoft\Windows\CurrentVersion\Uninstall\%~2 /F
rem %REG_EXEC% delete HKLM\SOFTWARE\Installer\Dependencies\%~2 /F
%REG_EXEC% delete HKLM\SOFTWARE\Classes\Installer\Dependencies\%~2 /F
)




