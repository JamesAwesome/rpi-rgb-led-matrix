# RGBMATRIX library cmake

# Build with some guidance from
# - https://github.com/scikit-build/scikit-build-sample-projects/
# - https://runebook.dev/en/docs/cmake/command/install

cmake_minimum_required(VERSION 3.22)
project(
    rgbmatrixlib
    VERSION "0.0.1"
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(CMakePrintHelpers) # so we can print cmake variable values

cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../cmake")

# Define the RGBMATRIX headers.
set(RGBMATRIX_HEADERS
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/canvas.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/content-streamer.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/graphics.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/led-matrix.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/led-matrix-c.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/pixel-mapper.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/thread.h
    ${CMAKE_CURRENT_SOURCE_DIR}/../include/threaded-canvas-manipulator.h
)
cmake_print_variables(RGBMATRIX_HEADERS)

# Define the RGBMATRIX source files
set(RGBMATRIX_SOURCES
    ${CMAKE_CURRENT_SOURCE_DIR}/bdf-font.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/content-streamer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/framebuffer.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/gpio.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/graphics.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/hardware-mapping.c
    ${CMAKE_CURRENT_SOURCE_DIR}/led-matrix.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/led-matrix-c.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/multiplex-mappers.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/options-initialize.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/pixel-mapper.cc
    ${CMAKE_CURRENT_SOURCE_DIR}/thread.cc
)
cmake_print_variables(RGBMATRIX_SOURCES)

# Declare the library
if(PROJECT_IS_TOP_LEVEL)
    add_library(rgbmatrixlib)
endif()

# Add the sources
target_sources(rgbmatrixlib PRIVATE ${RGBMATRIX_SOURCES})

# Add include directories
target_include_directories(rgbmatrixlib PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include> )

# Install the library.
install(
    TARGETS rgbmatrixlib
    EXPORT rgbmatrixlibTargets
    FILE_SET HEADERS
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

####
#
# cmake install files, so other packages can find this library.
#
####
set(RGBMATRIX_CMAKE_PACKAGE_INSTALL_SUBDIR "share/rgbmatrixlib/cmake")
cmake_print_variables(RGBMATRIX_CMAKE_PACKAGE_INSTALL_SUBDIR)
cmake_print_variables(CMAKE_INSTALL_SYSCONFDIR)
set(SYSCONFIG_INSTALL_DIR ${CMAKE_INSTALL_SYSCONFDIR}
    CACHE PATH "Location of configuration files" )
install(
  EXPORT rgbmatrixlibTargets  # this name before Targets should match the name of the variable set above in a previous install call
  NAMESPACE rgbmatrix::
  DESTINATION ${RGBMATRIX_CMAKE_PACKAGE_INSTALL_SUBDIR})

include(CMakePackageConfigHelpers)

write_basic_package_version_file(
  rgbmatrixlibConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

set(INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_INCLUDEDIR})  #set so we can use in configure_package_config_file()
set(LIB_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}) # ditto

configure_package_config_file(
  "${PROJECT_SOURCE_DIR}/../cmake/rgbmatrixlibConfig.cmake.in" rgbmatrixlibConfig.cmake
  PATH_VARS INCLUDE_INSTALL_DIR SYSCONFIG_INSTALL_DIR LIB_INSTALL_DIR
  INSTALL_DESTINATION ${RGBMATRIX_CMAKE_PACKAGE_INSTALL_SUBDIR}
  )

install(FILES "${PROJECT_BINARY_DIR}/rgbmatrixlibConfig.cmake"
              "${PROJECT_BINARY_DIR}/rgbmatrixlibConfigVersion.cmake"
        DESTINATION ${RGBMATRIX_CMAKE_PACKAGE_INSTALL_SUBDIR})
