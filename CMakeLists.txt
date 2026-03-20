# CMake for rpi-rgb-led-matrix

cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(rgbmatrix VERSION 0.0.1)
include(GNUInstallDirs)
find_package(Python 3.9 REQUIRED COMPONENTS Interpreter Development.Module)

# Build the core library
set(RGBMATRIX_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(RGBMATRIX_SOURCE_DIR ${CMAKE_SOURCE_DIR}/lib)

# Define the RGBMATRIX headers.
set(RGBMATRIX_HEADERS
    ${RGBMATRIX_INCLUDE_DIR}/canvas.h
    ${RGBMATRIX_INCLUDE_DIR}/content-streamer.h
    ${RGBMATRIX_INCLUDE_DIR}/graphics.h
    ${RGBMATRIX_INCLUDE_DIR}/led-matrix.h
    ${RGBMATRIX_INCLUDE_DIR}/led-matrix-c.h
    ${RGBMATRIX_INCLUDE_DIR}/pixel-mapper.h
    ${RGBMATRIX_INCLUDE_DIR}/thread.h
    ${RGBMATRIX_INCLUDE_DIR}/threaded-canvas-manipulator.h
)

# Define the RGBMATRIX source files
set(RGBMATRIX_SOURCES
    ${RGBMATRIX_SOURCE_DIR}/bdf-font.cc
    ${RGBMATRIX_SOURCE_DIR}/content-streamer.cc
    ${RGBMATRIX_SOURCE_DIR}/framebuffer.cc
    ${RGBMATRIX_SOURCE_DIR}/gpio.cc
    ${RGBMATRIX_SOURCE_DIR}/graphics.cc
    ${RGBMATRIX_SOURCE_DIR}/hardware-mapping.c
    ${RGBMATRIX_SOURCE_DIR}/led-matrix.cc
    ${RGBMATRIX_SOURCE_DIR}/led-matrix-c.cc
    ${RGBMATRIX_SOURCE_DIR}/multiplex-mappers.cc
    ${RGBMATRIX_SOURCE_DIR}/options-initialize.cc
    ${RGBMATRIX_SOURCE_DIR}/pixel-mapper.cc
    ${RGBMATRIX_SOURCE_DIR}/thread.cc
)

# Add the library
add_library(rgbmatrix SHARED)

set_property(TARGET rgbmatrix PROPERTY PREFIX "")
target_sources(rgbmatrix PRIVATE ${RGBMATRIX_SOURCES})
target_include_directories(rgbmatrix PUBLIC ${CMAKE_SOURCE_DIR}/include)

# Install location changes depending if called by scikit-build-core
install(TARGETS rgbmatrix DESTINATION ${CMAKE_INSTALL_LIBDIR})

# Build the python bindings if called by scikit-build-core
IF(DEFINED SKBUILD)

    # Convenience variable for the bindings path
    set(PYBINDING_SOURCE_DIR ${CMAKE_SOURCE_DIR}/bindings/python/rgbmatrix)
    # Transpile core
    add_custom_command(
        OUTPUT ${PYBINDING_SOURCE_DIR}/core.cpp
        COMMENT
            "Making ${PYBINDING_SOURCE_DIR}/core.cpp from ${PYBINDING_SOURCE_DIR}/core.pyx"
        COMMAND Python::Interpreter -m cython --cplus
            "${PYBINDING_SOURCE_DIR}/core.pyx" --output-file ${PYBINDING_SOURCE_DIR}/core.cpp
        DEPENDS ${PYBINDING_SOURCE_DIR}/core.pyx
        VERBATIM)

    # Transpile graphics
    add_custom_command(
      OUTPUT ${PYBINDING_SOURCE_DIR}/graphics.cpp
      COMMENT
        "Making ${PYBINDING_SOURCE_DIR}/graphics.cpp from ${PYBINDING_SOURCE_DIR}/graphics.pyx"
      COMMAND Python::Interpreter -m cython --cplus
        "${PYBINDING_SOURCE_DIR}/graphics.pyx" --output-file ${PYBINDING_SOURCE_DIR}/graphics.cpp
      DEPENDS ${PYBINDING_SOURCE_DIR}/graphics.pyx
      VERBATIM)

    # Make the core extension
    python_add_library(core SHARED
        ${PYBINDING_SOURCE_DIR}/core.cpp
        ${PYBINDING_SOURCE_DIR}/shims/pillow.c
        ${RGBMATRIX_SOURCES}
    )
    set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(core PRIVATE
        ${PYBINDING_SOURCE_DIR}/shims
        ${CMAKE_SOURCE_DIR}/include
    )
    set_property(TARGET core PROPERTY PREFIX "")

    # Make the graphics extension
    python_add_library(graphics SHARED
        ${PYBINDING_SOURCE_DIR}/graphics.cpp
        ${RGBMATRIX_SOURCES}
    )
    set_target_properties(graphics PROPERTIES LINKER_LANGUAGE CXX)
    target_include_directories(graphics PRIVATE
        ${CMAKE_SOURCE_DIR}/include
    )
    set_property(TARGET graphics PROPERTY PREFIX "")

    install(TARGETS core graphics DESTINATION ${SKBUILD_PROJECT_NAME})
    install(FILES "${PROJECT_SOURCE_DIR}/bindings/python/rgbmatrix/__init__.py" DESTINATION ${SKBUILD_PROJECT_NAME})
endif()
