# wxHighgui : Integrating opencv in wxwidgets
An OpenCV Graphical User interface using wxWidgets
You can build library using scripts. 

1 - You have to clone this repo and copy all scripts (.sh) in parent repo (=scriptREPO)

2 - You have to edit libmodel.sh script :
CMAKE_CONFIG_GENERATOR choose your compiler for cmake
updateRepo=ON means you will update your local repo using git (OFF = no update use it when there is no internet)
myRepo=$(pwd) where you want to clone all repo zlib, libpng wxwidgets and opencv
installRepo=${myRepo}/install  where you want to install all repo zlib, libpng wxwidgets and opencv wxHighgui
buildRepo=${myRepo}/build where you want to install all repo zlib, libpng, wxwidgets, opencv, wxHighgui and sample
sourceRepo=${myRepo} normaly same than $myRepo
SharedLibs=OFF static build

3 - run in git-bash initzlibpng.sh

4 - run in git-bash buildocv.sh

5 - run in git-bash buildwxForOpencv.sh

5 - run in git-bash buildwxHighgui.sh

If you want to build sample copy wxHighgui/sample in scriptREPO and run buildwxHighguisample.sh





We don't need libtiff and libjeg (inside wxwidgets) and we use pnglib and zlib for wxwidgets and opencv
