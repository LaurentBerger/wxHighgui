#!/bin/bash -e
source ./libmodel.sh

CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64"
RepoSource=wxnano
cd Build/$RepoSource
cmake -G"$CMAKE_CONFIG_GENERATOR"   \
-DBUILD_SHARED_LIBS:BOOL=OFF \
-DwxWidgets_ROOT_DIR=${installRepo}/wxwidgets -DwxWidgets_LIB_DIR=${installRepo}/wxwidgets/lib/${wxlibs} \
-Dopencv_DIR=${installRepo}/opencv/${ocvlibs} \
-DCMAKE_INSTALL_PREFIX=${installRepo}/"$RepoSource"  ../../"$RepoSource"

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



