#!/bin/bash -e 
source ./libmodel.sh
librepo=zlib
CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64" 
if [ ! -d "$myRepo/$librepo" ]; 
then echo "cloning $librepo" 
git clone https://github.com/madler/zlib.git 
mkdir Build/$librepo 
mkdir Install/$librepo 
else 
cd $librepo 
#git pull --rebase 
#git submodule update --init 
cd .. 
fi 
RepoSource=$librepo 
pushd Build/$RepoSource 
CMAKE_OPTIONS= cmake -G"$CMAKE_CONFIG_GENERATOR" \
-DCMAKE_INSTALL_PREFIX=../../install/"$RepoSource" ../../"$RepoSource" 
cmake --build . --config release 
cmake --build . --target install --config release 
cmake --build . --config debug 
cmake --build . --target install --config debug 
popd


#!/bin/bash -e 
librepo=libpng
CMAKE_CONFIG_GENERATOR="Visual Studio 15 2017 Win64" 
if [ ! -d "$myRepo/$librepo" ]; 
then echo "cloning $librepo" 
git clone https://github.com/glennrp/libpng.git 
mkdir Build/$librepo 
mkdir Install/$librepo 
else 
cd $librepo 
#git pull --rebase 
#git submodule update --init 
cd .. 
fi 
RepoSource=$librepo 
pushd Build/$RepoSource 
CMAKE_OPTIONS= cmake -G"$CMAKE_CONFIG_GENERATOR" \
-DPNG_TESTS:BOOL=OFF -DAWK= \
-DZLIB_INCLUDE_DIR:PATH=${myRepo}/install/zlib/include -DZLIB_LIBRARY_DEBUG:FILEPATH=${myRepo}/install/zlib/lib/zlibd.lib -DZLIB_LIBRARY_RELEASE:FILEPATH=${myRepo}/install/zlib/lib/zlib.lib \
-DCMAKE_INSTALL_PREFIX=../../install/"$RepoSource" ../../"$RepoSource" 
cmake --build . --config release 
cmake --build . --target install --config release 
cmake --build . --config debug 
cmake --build . --target install --config debug 
popd


