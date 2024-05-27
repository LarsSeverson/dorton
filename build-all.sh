set echo on

echo "Building everything"

pushd crates
make
popd

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
echo "ERROR:"$ERRORLEVEL && exit
fi

pushd src
make
popd

ERRORLEVEL=$?
if [ $ERRORLEVEL -ne 0 ]
then
echo "ERROR:"$ERRORLEVEL && exit
fi

echo "All assemblies built successfully."