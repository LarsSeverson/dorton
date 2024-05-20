@ECHO OFF
REM Build Everthing

ECHO "Building everything"

PUSHD engine
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && pause)

PUSHD testbed
CALL build.bat
POPD
IF %ERRORLEVEL% NEQ 0 (echo Error:%ERRORLEVEL% && pause)

ECHO "All assemblies build successfurly"