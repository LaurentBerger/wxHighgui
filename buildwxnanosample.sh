#!/bin/bash -e
myRepo=$(pwd)

CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64"
RepoSource=wxNano/sample
cd Build/$RepoSource
cmake -G"$CMAKE_CONFIG_GENERATOR"   \
-DBUILD_SHARED_LIBS:BOOL=OFF \
-DwxWidgets_ROOT_DIR=${myRepo}/install/wxwidgets -DwxWidgets_LIB_DIR=${myRepo}/install/wxwidgets/lib/vc_x64_lib \
-Dopencv_DIR=${myRepo}/install/opencv \
-DwxNanoLib_DIR=${myRepo}/install/wxnano/lib/cmake/wxNanoLib \
../../../"$RepoSource"

cd ../..

cmake --build $RepoSource  --config release 
echo "************************* $Source_DIR -->devenv debug"
cmake --build $RepoSource  --config debug 
cd ..



