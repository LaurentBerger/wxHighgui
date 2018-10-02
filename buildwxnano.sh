#!/bin/bash -e
myRepo=$(pwd)

CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64"
RepoSource=wxnano
cd Build/$RepoSource
cmake -G"$CMAKE_CONFIG_GENERATOR"   \
-DBUILD_SHARED_LIBS:BOOL=ON \
-DwxWidgets_ROOT_DIR=${myRepo}/install/wxwidgets -DwxWidgets_LIB_DIR=${myRepo}/install/wxwidgets/lib/vc_x64_dll \
-Dopencv_DIR=${myRepo}/install/opencv \
-DCMAKE_INSTALL_PREFIX=../../install/"$RepoSource"  ../../"$RepoSource"

cd ..

echo "************************* $Source_DIR -->devenv debug"
cmake --build $RepoSource  --config release 
echo "************************* $Source_DIR -->devenv release"
cmake --build $RepoSource  --target install --config release 
echo "************************* $Source_DIR -->devenv debug"
cmake --build $RepoSource  --config debug 
echo "************************* $Source_DIR -->devenv release"
cmake --build $RepoSource  --target install --config debug 
cd ..



