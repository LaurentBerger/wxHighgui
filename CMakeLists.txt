cmake_minimum_required(VERSION 3.0)


set(NomProjet wxHighgui)
set(CMAKE_SUPPRESS_REGENERATION true)
PROJECT (${NomProjet})
option(BUILD_SHARED_LIBS "Build shared library" ON)
find_package(wxWidgets COMPONENTS core base REQUIRED)
find_package(opencv)

set(wxHighguiSample OFF CACHE BOOL "wxHighguiLib must be already installed")
set(wxHighguiLib_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(wxHighguiLib_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(wxHighguiLib_VERSION ${wxHighguiLib_VERSION_MAJOR}.${wxHighguiLib_VERSION_MINOR} CACHE STRING "version" FORCE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

if (NOT BUILD_SHARED_LIBS)
find_package(glog )
find_package(gflags )
find_package(ceres )
endif(NOT BUILD_SHARED_LIBS)
if (WIN32)
	if (BUILD_SHARED_LIBS)
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /MD")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MDd")
      SET(wxWidgets_DEFINITIONS "WXUSINGDLL;__WXMSW__" )
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


add_library (wxHighguiLib STATIC ${wxHighguiLib_SRCS})
set_target_properties(wxHighguiLib PROPERTIES  COMPILE_DEFINITIONS "${wxWidgets_DEFINITIONS};_CRT_SECURE_NO_WARNINGS")

if (wxWidgets_FOUND AND OPENCV_FOUND)
	include_directories(${wxWidgets_INCLUDE_DIRS} ${OpenCV_INCLUDE_DIRS})
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

install(TARGETS wxHighguiLib EXPORT wxHighguiLibConfig
  LIBRARY DESTINATION "lib/\${CMAKE_INSTALL_CONFIG_NAME}"
  ARCHIVE DESTINATION "lib/\${CMAKE_INSTALL_CONFIG_NAME}"
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

install(EXPORT wxHighguiLibConfig
  FILE
    wxHighguiLibConfig.cmake
  DESTINATION
    ${ConfigPackageLocation}
)
#https://stackoverflow.com/questions/29971026/generator-expression-for-install-commands

install(SCRIPT cmake/patch_export_files.cmake DESTINATION
    ${ConfigPackageLocation})
    
if (wxHighguiSample)
ADD_SUBDIRECTORY(sample)
endif(wxHighguiSample)
