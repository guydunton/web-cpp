cmake_minimum_required(VERSION 3.8)
project(web-cpp)

if (NOT MSVC)
	option(ENABLE_COVERAGE "Enable coverage reporting for gcc/clang" FALSE)
endif()

enable_testing()

add_subdirectory(example)
add_subdirectory(test)