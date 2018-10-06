#!/bin/bash -e 
source ./libmodel.sh
if [[ $updateRepo = ON ]] 
then
	if [ ! -d "$myRepo/wxwidgets" ]; 
	then 
		echo "cloning wxwidgets" 
		git clone https://github.com/wxWidgets/wxWidgets.git 
		mkdir ${buildRepo}/wxwidgets 
		mkdir ${installRepo}/wxwidgets 
		git submodule update --init 
	else 
		cd wxwidgets 
		git pull --rebase 
	fi 
	cd ..
fi
RepoSource=wxwidgets 
pushd ${buildRepo}/$RepoSource 
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
-DCMAKE_INSTALL_PREFIX=${installRepo}/${RepoSource} ${sourceRepo}/"$RepoSource" 

cmake --build . --config release 
cmake --build . --target install --config release 
cmake --build . --config debug 
cmake --build . --target install --config debug 
popd