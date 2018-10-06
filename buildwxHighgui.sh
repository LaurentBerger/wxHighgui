#!/bin/bash -e
source ./libmodel.sh

RepoSource=wxHighgui
pushd Build/$RepoSource
cmake -G"$CMAKE_CONFIG_GENERATOR"   \
-DBUILD_SHARED_LIBS:BOOL=${SharedLibs} \
-DwxWidgets_ROOT_DIR=${installRepo}/wxwidgets -DwxWidgets_LIB_DIR=${installRepo}/wxwidgets/lib/${wxlibs} \
-Dopencv_DIR=${installRepo}/opencv/${ocvlibs} \
-DCMAKE_INSTALL_PREFIX=${installRepo}/"$RepoSource"  ${sourceRepo}/"$RepoSource"

popd

echo "************************* $Source_DIR -->devenv debug"
cmake --build .  --config release 
echo "************************* $Source_DIR -->devenv release"
cmake --build .  --target install --config release 
echo "************************* $Source_DIR -->devenv debug"
cmake --build .  --config debug 
echo "************************* $Source_DIR -->devenv release"
cmake --build .  --target install --config debug 
cd ..



