cmake_minimum_required(VERSION 3.0)


set(NomProjet wxHighgui)
set(CMAKE_SUPPRESS_REGENERATION true)
PROJECT (${NomProjet})
option(BUILD_SHARED_LIBS "Build shared library" ${SharedLibs})

find_package(wxWidgets COMPONENTS core base REQUIRED)
find_package(opencv)

set(wxHighguiSample OFF CACHE BOOL "wxHighguiLib must be already installed")
set(wxHighguiLib_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(wxHighguiLib_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(wxHighguiLib_VERSION ${wxHighguiLib_VERSION_MAJOR}.${wxHighguiLib_VERSION_MINOR} CACHE STRING "version" FORCE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)
# https://stackoverflow.com/questions/49672264/cmake-add-d-suffix-for-debug-build-of-static-library
set(CMAKE_DEBUG_POSTFIX d)
set(CMAKE_STATIC_LIBRARY_SUFFIX_CXX _static.lib)
if (NOT BUILD_SHARED_LIBS)
find_package(glog )
find_package(gflags )
find_package(ceres )
endif(NOT BUILD_SHARED_LIBS)
SET (LIB_TYPE STATIC)
if (WIN32)
	if (BUILD_SHARED_LIBS)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
      SET(wxWidgets_DEFINITIONS "WXUSINGDLL;__WXMSW__" )
	SET (LIB_TYPE SHARED)
	else (BUILD_SHARED_LIBS)
      SET(wxWidgets_DEFINITIONS "__WXMSW__" )
	endif (BUILD_SHARED_LIBS)
else (WIN32)
	if (BUILD_SHARED_LIBS)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	else (BUILD_SHARED_LIBS)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
	endif (BUILD_SHARED_LIBS)
endif (WIN32)

include(GenerateExportHeader)


file(GLOB wxHighguiLib_SRCS     "src/*.cpp")
file(GLOB wxHighguiLib_Headers  "include/*.h" "include/*.hpp")

message ("wxHighguiLib_VERSION ${wxHighguiLib_VERSION}")
message ("CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}")
message ("CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
message ("CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES}")

add_library (wxHighguiLib ${LIB_TYPE} ${wxHighguiLib_SRCS})
set_target_properties(wxHighguiLib PROPERTIES  COMPILE_DEFINITIONS "${wxWidgets_DEFINITIONS};_CRT_SECURE_NO_WARNINGS")

if (wxWidgets_FOUND AND OPENCV_FOUND)
	include_directories(${wxWidgets_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
	if (BUILD_SHARED_LIBS)
	   target_link_libraries (wxHighguiLib LINK_PUBLIC  ${OpenCV_LIBS} ${wxWidgets_LIBRARIES} )
	   set(configSuffix )
	else (BUILD_SHARED_LIBS)
	   set(configSuffix Static)
	endif(BUILD_SHARED_LIBS)
endif (wxWidgets_FOUND AND OPENCV_FOUND)

if (DEFINED CMAKE_CONFIGURATION_TYPES)
  # multi-config handling
  set (CMAKE_BUILD_CONFIG \${BUILD_TYPE})
  set (CMAKE_CFG_BUILD_CONFIG ${CMAKE_CFG_INTDIR})
else()
  # mono config handling
  if (CMAKE_BUILD_TYPE)
     set (CMAKE_BUILD_TYPE ${CMAKE_BUILD_TYPE} CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel" FORCE)
  else()
     set (CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build, options are: None(CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) Debug Release RelWithDebInfo MinSizeRel" FORCE)
  endif()
  set (CMAKE_BUILD_CONFIG ${CMAKE_BUILD_TYPE})
  set (CMAKE_CFG_BUILD_CONFIG ${CMAKE_BUILD_TYPE})
endif()


add_custom_command (OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/${CMAKE_CFG_BUILD_CONFIG}/output_file"
                    COMMAND "${CMAKE_COMMAND}" -DBUILD_TYPE=$<CONFIG> -P script.cmake
                    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
                    COMMENT "Generating output_file")


target_include_directories(wxHighguiLib PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	PRIVATE source)

generate_export_header( wxHighguiLib )

set_property(TARGET wxHighguiLib PROPERTY VERSION ${wxHighguiLib_VERSION})
set(CMAKE_INSTALL_CONFIG_NAME "${CMAKE_INSTALL_CONFIG_NAME}${configSuffix}")
set(configName "wxHighguiLib${configSuffix}Config.cmake")
install(TARGETS wxHighguiLib EXPORT wxHighguiLibConfig
  LIBRARY DESTINATION "lib"
  ARCHIVE DESTINATION "lib"
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)


install(FILES "${CMAKE_CURRENT_BINARY_DIR}/wxHighguiLib_export.h"  DESTINATION include   COMPONENT    Devel)
install(FILES ${wxHighguiLib_Headers} DESTINATION include)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/wxHighguiLib/wxHighguiLibConfigVersion.cmake"
  VERSION ${wxHighguiLib_VERSION}
  COMPATIBILITY AnyNewerVersion
)


set(ConfigPackageLocation lib/cmake/wxHighguiLib)

message ("ConfigPackageLocation ${ConfigPackageLocation}")
message ("configSuffix ${configSuffix}")

install(EXPORT wxHighguiLibConfig
  FILE
    ${configName}
  DESTINATION
    ${ConfigPackageLocation}
)
#https://stackoverflow.com/questions/29971026/generator-expression-for-install-commands

install(SCRIPT cmake/patch_export_files.cmake DESTINATION
    ${ConfigPackageLocation})
    
if (wxHighguiSample)
ADD_SUBDIRECTORY(sample)
endif(wxHighguiSample)
