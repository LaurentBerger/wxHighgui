cmake_minimum_required(VERSION 3.0)


set(NomProjet wxNanoPrj)
PROJECT (${NomProjet})
find_package(wxWidgets COMPONENTS core base net adv aui html  qa richtext stc ribbon xml gl REQUIRED)

set(wxNano_VERSION_MAJOR 0 CACHE STRING "major version" FORCE)
set(wxNano_VERSION_MINOR 0 CACHE STRING "minor version" FORCE)
set(wxNano_VERSION ${wxNano_VERSION_MAJOR}.${wxNano_VERSION_MINOR} CACHE STRING "version" FORCE)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_CURRENT_DIR_IN_INTERFACE ON)

include(GenerateExportHeader)


file(GLOB wxNano_SRCS     "src/*.cpp")
file(GLOB wxNano_Headers  "include/*.h" "include/*.hpp")

message ("wxNano_VERSION ${wxNano_VERSION}")
message ("CMAKE_CURRENT_BINARY_DIR ${CMAKE_CURRENT_BINARY_DIR}")
message ("CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}")
message ("CMAKE_CONFIGURATION_TYPES ${CMAKE_CONFIGURATION_TYPES}")


add_library (wxNano STATIC ${wxNano_SRCS})

if (wxWidgets_FOUND)
	include_directories(${wxWidgets_INCLUDE_DIRS})
endif (wxWidgets_FOUND)

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


target_include_directories(wxNano PUBLIC
	$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
	$<INSTALL_INTERFACE:include>
	PRIVATE source)

generate_export_header( wxNano )

set_property(TARGET wxNano PROPERTY VERSION ${wxNano_VERSION})

install(TARGETS wxNano EXPORT wxNanoConfig
  LIBRARY DESTINATION "lib/\${CMAKE_INSTALL_CONFIG_NAME}"
  ARCHIVE DESTINATION "lib/\${CMAKE_INSTALL_CONFIG_NAME}"
  RUNTIME DESTINATION bin
  INCLUDES DESTINATION include
)


install(FILES "${CMAKE_CURRENT_BINARY_DIR}/wxNano_export.h"  DESTINATION include   COMPONENT    Devel)
install(FILES ${wxNano_Headers} DESTINATION include)

include(CMakePackageConfigHelpers)
write_basic_package_version_file(
  "${CMAKE_CURRENT_BINARY_DIR}/wxNano/wxNanoConfigVersion.cmake"
  VERSION ${wxNano_VERSION}
  COMPATIBILITY AnyNewerVersion
)


set(ConfigPackageLocation lib/cmake/wxNano)

message ("ConfigPackageLocation ${ConfigPackageLocation}")

install(EXPORT wxNanoConfig
  FILE
    wxNanoConfig.cmake
  DESTINATION
    ${ConfigPackageLocation}
)
#https://stackoverflow.com/questions/29971026/generator-expression-for-install-commands

install(SCRIPT cmake/patch_export_files.cmake DESTINATION
    ${ConfigPackageLocation})