cmake_minimum_required(VERSION 3.8)
project(web-tests)

add_executable(web-tests 
	main.cpp
	basic-test.cpp
    meta-tests.cpp
)

target_include_directories(web-tests PUBLIC ../include)

target_compile_features(web-tests PRIVATE cxx_std_17)

if (MSVC)
	target_compile_options(web-tests PUBLIC /W4 /std:c++17 /permissive-)
else()
	target_compile_options(web-tests PUBLIC -Wall -Wextra -Wpedantic)
	
	if (ENABLE_COVERAGE)
		target_compile_options(web-tests PUBLIC --coverage -O0)
	endif()
endif()

if (NOT MSVC)
	target_link_libraries(web-tests --coverage)
endif()

add_test(Test web-tests)