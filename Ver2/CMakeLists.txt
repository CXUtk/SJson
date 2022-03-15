cmake_minimum_required (VERSION 3.8)

project ("SJson")

add_executable (SJsonTest "test.cpp")

set_target_properties (SJsonTest PROPERTIES
                      CXX_STANDARD 20
                      CXX_EXTENSIONS OFF
                      )
