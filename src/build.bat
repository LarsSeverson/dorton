REM Build script for dprton 
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get list of all .c files
SET cFileNames=
FOR /R %%f in (*.c) do (
  SET cFileNames=!cFileNames! %%f
)

REM echo "Files:" %cFileNames%

SET assembly=dorton
SET compilerFlags=-g 
REM -Wall -Werror
SET includeFlags=-I../crates/app/ -I../crates/internal/ -I%GLFW%\include -I%VULKAN_SDK%/Include
SET linkerFlags=-L../bin/ -lcrates.lib
SET defines=-D_DEBUG -DDIMPORT

ECHO "Building %assembly%%..."
clang %cFileNames% %compilerFlags% -o ../bin/%assembly%.exe %defines% %includeFlags% %linkerFlags%