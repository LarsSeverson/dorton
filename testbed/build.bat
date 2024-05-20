REM Build script for bed 
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get list of all .c files
SET cFileNames=
FOR /R %%f in (*.c) do (
  SET cFileNames=!cFileNames! %%f
)

REM echo "Files:" %cFileNames%

SET assembly=testbed
SET compilerFlags=-g 
REM -Wall -Werror
SET includeFlags=-Isrc -I../engine/src/ -I%GLFW%\include
SET linkerFlags=-L../bin/ -lengine.lib
SET defines=-D_DEBUG -DDIMPORT

ECHO "Building %assembly%%..."
clang %cFileNames% %compilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%