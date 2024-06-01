REM Build script for crates
@ECHO OFF
SetLocal EnableDelayedExpansion

REM Get list of all .c files
SET cFileNames=
FOR /R %%f in (*.c) do (
  SET cFileNames=!cFileNames! %%f
)

REM echo "Files:" %cFileNames%

SET assembly=crates
SET compilerFlags=-g -shared -Wvarargs -Wall -Werror
REM -Wall -Werror
SET includeFlags= -Iinternal -Idorton_utils -Ilogger -I%VULKAN_SDK%/Include -I%GLFW%\include
SET linkerFlags=-luser32 -lvulkan-1 -L%VULKAN_SDK%/Lib -L%GLFW%/lib -lglfw3_mt -lopengl32 -lshell32 -lgdi32
SET defines=-D_DEBUG -DDEXPORT -D_CRT_SECURE_NO_WARNINGS

ECHO "Building %assembly%%..."
clang %cFileNames% %compilerFlags% -o ../bin/%assembly%.dll %defines% %includeFlags% %linkerFlags%