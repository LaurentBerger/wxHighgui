#!/bin/bash -e
myRepo=$(pwd)

CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64"
RepoSource=wxnano
cd Build/$RepoSource
CMAKE_OPTIONS='-DBUILD_opencv_world:BOOL=OFF -DBUILD_PERF_TESTS:BOOL=OFF -DBUILD_TESTS:BOOL=ON -DBUILD_DOCS:BOOL=ON -DWITH_CUDA:BOOL=OFF -DBUILD_EXAMPLES:BOOL=ON -DWITH_LAPACK:BOOL=ON -DCV_TRACE:BOOL=OFF -DBUILD_PNG:BOOL=OFF -DBUILD_ZLIB:BOOL=OFF '
cmake -G"$CMAKE_CONFIG_GENERATOR"   \
-DBUILD_SHARED_LIBS:BOOL=OFF \
-DwxWidgets_ROOT_DIR=${repo}/install/wxWidgets -DwxWidgets_LIB_DIR=${repo}/install/wxWidgets/lib/vc_x64_lib \
-Dopencv_DIR=${repo}/install/opencv \
-DINSTALL_CREATE_DISTRIB=ON -DCMAKE_INSTALL_PREFIX=../../install/"$RepoSource"  ../../"$RepoSource"

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



