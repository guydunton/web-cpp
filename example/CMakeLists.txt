cmake_minimum_required(VERSION 3.8)
project(web-examples)

add_executable(web-examples main.cpp)

target_include_directories(web-examples PUBLIC ../include)

target_compile_features(web-examples PRIVATE cxx_std_17)

if (MSVC)
	target_compile_options(web-examples PUBLIC /W4 /std:c++17 /permissive-)
else()
	target_compile_options(web-examples PUBLIC -Wall -Wextra -Wpedantic)
endif()