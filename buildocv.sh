#!/bin/bash -e
myRepo=$(pwd)

librepo=opencv_contrib
CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64" 
if [ ! -d "$myRepo/$librepo" ]; 
then echo "cloning $librepo" 
git clone https://github.com/opencv/opencv_contrib.git 
mkdir Build/$librepo 
mkdir Install/$librepo 
else 
cd $librepo 
git pull --rebase 
cd .. 
fi 

librepo=opencv
CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64" 
if [ ! -d "$myRepo/$librepo" ]; 
then echo "cloning $librepo" 
git clone https://github.com/opencv/opencv.git 
mkdir Build/$librepo 
mkdir Install/$librepo 
else 
cd $librepo 
git pull --rebase 
cd .. 
fi 


CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64"
RepoSource=opencv
cd Build/$RepoSource
CMAKE_OPTIONS='-DBUILD_opencv_world:BOOL=OFF -DBUILD_PERF_TESTS:BOOL=OFF -DBUILD_TESTS:BOOL=ON -DBUILD_DOCS:BOOL=ON -DWITH_CUDA:BOOL=OFF -DBUILD_EXAMPLES:BOOL=ON -DWITH_LAPACK:BOOL=ON -DCV_TRACE:BOOL=OFF -DBUILD_PNG:BOOL=OFF -DBUILD_ZLIB:BOOL=OFF '
cmake -G"$CMAKE_CONFIG_GENERATOR"   \
-DZLIB_INCLUDE_DIR:PATH=${myRepo}/install/zlib/include -DZLIB_LIBRARY_DEBUG:FILEPATH=${myRepo}/install/zlib/lib/zlibd.lib -DZLIB_LIBRARY_RELEASE:FILEPATH=${myRepo}/install/zlib/lib/zlib.lib \
-DPNG_PNG_INCLUDE_DIR:PATH=${myRepo}/install/libpng/include -DPNG_LIBRARY_RELEASE:FILEPATH=${myRepo}/install/libpng/lib/libpng16.lib -DPNG_LIBRARY_DEBUG:FILEPATH=${myRepo}/install/lpng/lib/libpng16d.lib \
$CMAKE_OPTIONS -DOPENCV_EXTRA_MODULES_PATH=../../opencv_contrib/modules -DOPENCV_ENABLE_NONFREE:BOOL=ON \
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



