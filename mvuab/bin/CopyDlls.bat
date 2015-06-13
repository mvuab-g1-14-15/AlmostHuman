@echo off

echo Copying dlls
if !%1!==!""! goto INVALID_EXE_DIRECTORY
if NOT EXIST %1 goto DIR_NOT_EXIST

SET bat_dir="%~dp0"
SET exe_directory=%1
SET XCOPY_BIN=%SystemRoot%\system32\xcopy.exe

%XCOPY_BIN% %bat_dir%\\dlls\\*.* %exe_directory%  /F /R /Y

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
