SharedLibs=ON
zlibname=zlib
pnglibname=libpng16
if [[ $SharedLibs = OFF ]]
then
zlibname=${zlibname}static
pnglibname=${pnglibname}_static
fi
