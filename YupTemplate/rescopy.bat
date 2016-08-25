@echo off

IF NOT "%RES_FILES%" == "" (
echo Copying DLLs and resources to output folder...
FOR %%I IN (%RES_FILES%) DO (
xcopy %%I "%OUT_DIR%" /D /Y /S
)

)