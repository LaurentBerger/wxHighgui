cmake_minimum_required(VERSION 3.0)

set(NomProjet TDwx)
PROJECT (${NomProjet})
find_package(opencv  REQUIRED)
find_package(wxWidgets COMPONENTS core base net adv aui html  qa richtext stc ribbon xml gl REQUIRED)
find_package(wxNanoLib REQUIRED)

file(GLOB Projet_SRCS
    "*.h"
    "*.cpp")

ADD_EXECUTABLE (${NomProjet} ${Projet_SRCS} )

if (wxWidgets_FOUND AND opencv_FOUND AND wxNanoLib_FOUND)
	include_directories(${wxWidgets_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
	target_link_libraries (${NomProjet} LINK_PUBLIC  ${OpenCV_LIBS} ${wxWidgets_LIBRARIES} wxNano)
endif (wxWidgets_FOUND)
SET(wxWidgets_DEFINITIONS WXUSINGDLL)
set_target_properties(${NomProjet} PROPERTIES  COMPILE_DEFINITIONS "${wxWidgets_DEFINITIONS};__WXMSW__;_CRT_SECURE_NO_WARNINGS")
set_target_properties(${NomProjet} PROPERTIES LINK_FLAGS_DEBUG  "/SUBSYSTEM:CONSOLE")
set_target_properties(${NomProjet} PROPERTIES LINK_FLAGS_RELEASE  "/SUBSYSTEM:CONSOLE")		


