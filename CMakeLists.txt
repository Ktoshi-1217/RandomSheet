cmake_minimum_required(VERSION 3.16)
project(QtTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(main main.cpp)

target_link_libraries(main Qt6::Widgets)

