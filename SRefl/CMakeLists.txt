cmake_minimum_required (VERSION 3.8)

project ("SRefl")

add_executable (SReflTest "test.cpp" "SRefl.hpp")

set_target_properties (SReflTest PROPERTIES
                      CXX_STANDARD 20
                      CXX_EXTENSIONS OFF
                      )
