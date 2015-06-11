@echo off
echo Copying dlls
if "%2"=="" goto INVALID_EXE_DIRECTORY
if NOT EXIST %1 goto DIR_NOT_EXIST
if NOT EXIST %2 goto DIR_NOT_EXIST

SET exe_directory=%2

SET bat_dir=%0
SET bat_dir=%bat_dir:CopyDlls.bat=%

@rem En Bamboo no tenemos bien el PATH, asi que usamos directamente xcopy.exe
SET XCOPY_BIN=%SystemRoot%\system32\xcopy.exe 

%XCOPY_BIN% %bat_dir%\\DNET_Debug\\*.* %_outdir%  /F /R /Y

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
