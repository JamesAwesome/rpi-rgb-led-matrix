# this is the cmake file for the rgbmatrix library
# Build with some guidance from
# - https://github.com/scikit-build/scikit-build-sample-projects/
# - https://runebook.dev/en/docs/cmake/command/install

cmake_minimum_required(VERSION 3.22)
project(
    rgbmatrix
    VERSION "0.0.0"
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(CMakePrintHelpers) # so we can print cmake variable values

cmake_print_variables(CMAKE_CURRENT_SOURCE_DIR)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -fPIC -std=c++11")

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
add_library(rgbmatrix SHARED ${RGBMATRIX_SOURCES})

# see https://stackoverflow.com/questions/25676277/cmake-target-include-directories-prints-an-error-when-i-try-to-add-the-source
# for more on these generator expressions, $<INSTALL_INTERFACE, etc.

# Add include directories
target_include_directories(rgbmatrix PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include> )
#    $<INSTALL_INTERFACE:include>  # <prefix>/include
#    )

# target_include_directories(rgbmatrix PUBLIC $<INSTALL_INTERFACE:include>)

install(
    TARGETS rgbmatrix
    EXPORT rgbmatrix_targets
    # ARCHIVE DESTINATION "lib"
    # LIBRARY DESTINATION "lib"
    # COMPONENT library
    FILE_SET HEADERS
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

#install(
#    FILES ${RGBMATRIX_CORE_HEADERS}
#    DESTINATION "../include"
#)

# Export the file for use by bindings.
#install(EXPORT rgbmatrix_targets
#    FILE librgbmatrix.cmake
#    NAMESPACE rgbmatrix_targets.cmake
#    DESTINATION share/rgbmatrix)

###########################################
#
# set up some things for unit testing, which uses Boost.Unittest
#
##################################

# TODO make this conditional on wanting them actually built.
# see https://github.com/bertiniteam/b2/issues/202

#set(EXAMPLE_CORE_TEST_SOURCES
#    test/test_core.cpp
#)
#
#add_executable(test_core ${EXAMPLE_CORE_TEST_SOURCES})
#target_include_directories(test_core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/test)

#target_link_libraries(test_core ${Boost_LIBRARIES} example_core_lib)
#add_test(NAME test_core COMMAND ${CMAKE_BINARY_DIR}/test_core)

#enable_testing()
