#!/bin/bash -e 
myRepo=$(pwd) 
opencvInstall=${myRepo}/install/opencv
CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64" 
if [ ! -d "$myRepo/wxwidgets" ]; 
then echo "cloning wxwidgets" 
git clone https://github.com/wxWidgets/wxWidgets.git 
mkdir Build/wxwidgets 
mkdir Install/wxwidgets 
else 
cd wxwidgets 
git pull --rebase 
git submodule update --init 
cd .. 
fi 
RepoSource=wxwidgets 
pushd Build/$RepoSource 
CMAKE_OPTIONS= cmake -G"$CMAKE_CONFIG_GENERATOR" \
-DwxBUILD_SHARED:BOOL=OFF -DwxBUILD_USE_STATIC_RUNTIME:BOOL=ON -DwxBUILD_COMPATIBILITY=3.1 \
-DwxUSE_LIBTIFF=OFF -DwxUSE_LIBJPEG=OFF -DwxUSE_EXPAT=OFF -DwxUSE_XRC=OFF \
-DwxUSE_ZLIB=sys -DZLIB_INCLUDE_DIR=${myRepo}/install/zlib/include \
-DZLIB_LIBRARY_DEBUG=${myRepo}/install/zlib/lib/zlibd.lib \
-DZLIB_LIBRARY_RELEASE=${myRepo}/install/zlib/lib/zlib.lib \
-DwxUSE_LIBPNG=sys -DPNG_PNG_INCLUDE_DIR=${myRepo}/install/libpng/include \
-DPNG_LIBRARY_DEBUG=${myRepo}/install/libpng/libpngd.lib \
-DPNG_LIBRARY_RELEASE=${myRepo}/install/libpng/libpng.lib \
-DCMAKE_INSTALL_PREFIX=../../install/"$RepoSource" ../../"$RepoSource" 
cmake --build . --config release 
cmake --build . --target install --config release 
cmake --build . --config debug 
cmake --build . --target install --config debug 
popd