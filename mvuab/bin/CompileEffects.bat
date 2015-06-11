@echo off
echo Compilling effects

SET bat_dir=%~dp0

set fxc_bin=%DXSDK_DIR%/Utilities/bin/x86/fxc.exe

echo %fxc_bin%

%fxc_bin%

rem if "%1"=="" goto INVALID_EXE_DIRECTORY
rem if NOT EXIST %1 goto DIR_NOT_EXIST

rem SET exe_directory=%1

rem SET bat_dir=%~dp0

rem SET XCOPY_BIN=%DXSDK_DIR%\system32\xcopy.exe

rem %XCOPY_BIN% %bat_dir%\\dlls\\*.* %exe_directory%  /F /R /Y

goto END

:INVALID_DLLS_DIRECTORY
echo Invalid dll's directory
goto END

:INVALID_EXE_DIRECTORY
echo Invalid exe directory 
goto END

:DIR_NOT_EXIST
echo The directory does not exist
goto END

:END
@echo off