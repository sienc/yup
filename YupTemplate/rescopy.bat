@echo off

::Define newline variable
(SET LF=^
%=this line is empty=%
)

IF NOT "%RES_FILES%" == "" (
echo Copying DLLs and resources to output folder...

:: Save file list to a temporary file, replacing ; with newline
echo %RES_FILES:;=!LF!% > rescopy.tmp

:: Loop through items in the file
FOR /F %%I IN (rescopy.tmp) DO (
echo Copying %%I
xcopy %%I "%OUT_DIR%" /D /Y /S
)

:: delete temp file
del rescopy.tmp

)