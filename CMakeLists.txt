cmake_minimum_required(VERSION 3.0)


set(NomProjet wxNano)
PROJECT (${NomProjet})
find_package(wxWidgets COMPONENTS core base net adv aui html  qa richtext stc ribbon xml gl REQUIRED)
find_package(opencv)

set(wxNanoSample OFF CACHE BOOL "wxNanoLib must be already installed")
set(wxNanoLib_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(wxNanoLib_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(wxNanoLib_VERSION ${wxNanoLib_VERSION_MAJOR}.${wxNanoLib_VERSION_MINOR} CACHE STRING "version" FORCE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include(GenerateExportHeader)


file(GLOB wxNanoLib_SRCS     "src/*.cpp")
file(GLOB wxNanoLib_Headers  "include/*.h" "include/*.hpp")

message ("wxNanoLib_VERSION ${wxNanoLib_VERSION}")
message ("CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}")
message ("CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
message ("CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES}")


add_library (wxNanoLib STATIC ${wxNanoLib_SRCS})

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


target_include_directories(wxNanoLib PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	PRIVATE source)

generate_export_header( wxNanoLib )

set_property(TARGET wxNanoLib PROPERTY VERSION ${wxNanoLib_VERSION})

install(TARGETS wxNanoLib EXPORT wxNanoLibConfig
  LIBRARY DESTINATION "lib/\${CMAKE_INSTALL_CONFIG_NAME}"
  ARCHIVE DESTINATION "lib/\${CMAKE_INSTALL_CONFIG_NAME}"
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)


install(FILES "${CMAKE_CURRENT_BINARY_DIR}/wxNanoLib_export.h"  DESTINATION include   COMPONENT    Devel)
install(FILES ${wxNanoLib_Headers} DESTINATION include)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/wxNanoLib/wxNanoLibConfigVersion.cmake"
  VERSION ${wxNanoLib_VERSION}
  COMPATIBILITY AnyNewerVersion
)


set(ConfigPackageLocation lib/cmake/wxNanoLib)

message ("ConfigPackageLocation ${ConfigPackageLocation}")

install(EXPORT wxNanoLibConfig
  FILE
    wxNanoLibConfig.cmake
  DESTINATION
    ${ConfigPackageLocation}
)
#https://stackoverflow.com/questions/29971026/generator-expression-for-install-commands

install(SCRIPT cmake/patch_export_files.cmake DESTINATION
    ${ConfigPackageLocation})
    
if (wxNanoSample)
ADD_SUBDIRECTORY(sample)
endif(wxNanoSample)
