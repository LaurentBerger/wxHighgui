#!/bin/bash -e 
SharedLibs=ON
zlibname=zlib
pnglibname=libpng16
if [[ $SharedLibs = OFF ]]
then
zlibname=${zlibname}static
pnglibname=${pnglibname}_static
fi
myRepo=$(pwd) 
installRepo=${myRepo}/install
CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64" 
if [ ! -d "$myRepo/wxwidgets" ]; 
then echo "cloning wxwidgets" 
git clone https://github.com/wxWidgets/wxWidgets.git 
mkdir Build/wxwidgets 
mkdir Install/wxwidgets 
else 
cd wxwidgets 
git pull --rebase 
fi 
git submodule update --init 
cd ..
RepoSource=wxwidgets 
pushd Build/$RepoSource 
cmake -G"$CMAKE_CONFIG_GENERATOR" \
-DwxBUILD_SHARED:BOOL=${SharedLibs} -DwxBUILD_USE_STATIC_RUNTIME:BOOL=OFF -DwxBUILD_COMPATIBILITY=3.1 \
-DwxUSE_LIBTIFF=OFF -DwxUSE_LIBJPEG=OFF \
-DwxUSE_EXPAT=builtin \
-DwxUSE_ZLIB=sys -DZLIB_INCLUDE_DIR=${installRepo}/zlib/include \
-DZLIB_LIBRARY_DEBUG=${installRepo}/zlib/lib/${zlibname}d.lib \
-DZLIB_LIBRARY_RELEASE=${installRepo}/zlib/lib/${zlibname}.lib \
-DwxUSE_LIBPNG=sys -DPNG_PNG_INCLUDE_DIR=${installRepo}/libpng/include \
-DPNG_LIBRARY_DEBUG=${installRepo}/libpng/lib/${pnglibname}d.lib \
-DPNG_LIBRARY_RELEASE=${installRepo}/libpng/lib/${pnglibname}.lib \
-DCMAKE_INSTALL_PREFIX==${installRepo}/"$RepoSource" ../../"$RepoSource" 
sdsdsdqsq
cmake --build . --config release 
cmake --build . --target install --config release 
cmake --build . --config debug 
cmake --build . --target install --config debug 
popd