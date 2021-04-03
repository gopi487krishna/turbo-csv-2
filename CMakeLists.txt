cmake_minimum_required(VERSION 3.10)
project(turbo-csv2 VERSION 0.1.0 LANGUAGES CXX)

include(CTest)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-DEXAMPLES="${CMAKE_CURRENT_SOURCE_DIR}/test/test_files/")

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

find_path(BOOST_INCLUDE_DIR boost/test/unit_test.hpp)

include_directories(${BOOST_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

enable_testing()

add_subdirectory(test)