#!/bin/bash 
myRepo=$(pwd)
installRepo=${myRepo}/install
SharedLibs=OFF
zlibname=zlib
pnglibname=libpng16
wxlibs=vc_x64_lib
ocvlibs=x64/vc15/staticlib
if [[ $SharedLibs = OFF ]]
then
zlibname=${zlibname}static
pnglibname=${pnglibname}_static
wxlibs=vc_x64_dll
ocvlibs=
fi

