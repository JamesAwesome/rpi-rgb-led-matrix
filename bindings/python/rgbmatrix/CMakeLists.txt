# Python Binding CMAKE
cmake_minimum_required(VERSION 3.22)
project(rgbmatrix_python_bindings)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(CMakePrintHelpers) # so we can print cmake variable values
find_package(Python 3.13 COMPONENTS Interpreter Development.Module Development.Embed REQUIRED)

# Bring in Cython
find_package(Cython MODULE REQUIRED VERSION 3.0)
include(UseCython)

cmake_print_variables(CMAKE_INSTALL_LIBDIR)

find_package(rgbmatrixlib REQUIRED CONFIG)

# Transpile core
add_custom_command(
    OUTPUT core.cpp
    COMMENT
        "Making ${CMAKE_CURRENT_BINARY_DIR}/core.cpp from ${CMAKE_CURRENT_SOURCE_DIR}/core.pyx"
    COMMAND Python::Interpreter -m cython --cplus
         "${CMAKE_CURRENT_SOURCE_DIR}/core.pyx" --output-file core.cpp
    DEPENDS core.pyx
    VERBATIM)

# Transpile graphics
add_custom_command(
  OUTPUT graphics.cpp
  COMMENT
    "Making ${CMAKE_CURRENT_BINARY_DIR}/example.c from ${CMAKE_CURRENT_SOURCE_DIR}/graphics.pyx"
  COMMAND Python::Interpreter -m cython --cplus
          "${CMAKE_CURRENT_SOURCE_DIR}/graphics.pyx" --output-file graphics.cpp
  DEPENDS graphics.pyx
  VERBATIM)

# These paths should probably be specified with cmake path variables instead.
# Can fix later, just trying to get something that works.
python_add_library(core STATIC core.cpp)
set_target_properties(core PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(core
    PRIVATE ../../../include ./shims)
target_link_libraries(core PRIVATE rgbmatrixlib)
python_add_library(graphics STATIC graphics.cpp)
set_target_properties(graphics PROPERTIES LINKER_LANGUAGE CXX)
target_include_directories(graphics PRIVATE ../../../include)
target_link_libraries(graphics PRIVATE rgbmatrixlib)
python_add_library(rgbmatrix MODULE core graphics WITH_SOABI)
target_include_directories(rgbmatrix PRIVATE ../../../include ./shims)

if(${SKBUILD})
	install(TARGETS rgbmatrix_python_bindings LIBRARY DESTINATION .)
else()
	install(TARGETS rgbmatrix_python_bindings LIBRARY DESTINATION ${Python_SITELIB}/example)
endif()
