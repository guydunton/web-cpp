cmake_minimum_required(VERSION 3.0)
project(web-test)

add_executable(web-test main.cpp)

target_compile_features(web-test PRIVATE cxx_std_17)