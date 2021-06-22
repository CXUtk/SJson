cmake_minimum_required (VERSION 3.8)

project ("SJson")


add_executable (SJson "SJson.cpp" "SJson.h" "test.cpp" )


set_target_properties (SJson PROPERTIES
                      CXX_STANDARD 17
                      CXX_EXTENSIONS OFF
                      )
