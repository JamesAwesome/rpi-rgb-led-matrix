# CMake for rpi-rgb-led-matrix
# Derived from https://github.com/ofloveandhate/scikit-build_boost-python/
cmake_minimum_required(VERSION 3.22)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
project(rgbmatrix)
include(CMakePrintHelpers) # so we can print cmake variable values
include(GNUInstallDirs)  # so that `CMAKE_INSTALL_LIBDIR` is not blank. eg
find_package(Python 3.13 REQUIRED COMPONENTS Interpreter Development.Module)

# Build the rgbmatrix library and the demo.
add_library(rgbmatrixlib)
add_subdirectory(lib) #add_library(librgbmatrix.so SHARED lib)

# Build the Python Bindings
# This part doesn't work, it can't find the previously build package. Excluding it will build
# a package that just contains the static C++ library, which isn't what we need.
#python_add_library(rgbmatrix_python_bindings MODULE WITH_SOABI)
#set_property(TARGET rgbmatrix_python_bindings PROPERTY OUTPUT_NAME rgbmatrix)
#add_subdirectory(bindings/python/rgbmatrix)

