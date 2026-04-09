cmake_minimum_required(VERSION 3.16)
project(QtTest)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(Qt6_DIR "C:/msys64/mingw64/lib/cmake/Qt6")
find_package(Qt6 REQUIRED COMPONENTS Widgets)

add_executable(main
    src/main.cpp
    src/RandomTable.cpp
    src/Jsonio.cpp
)

target_include_directories(main PRIVATE
    "${CMAKE_SOURCE_DIR}/include"
)

target_link_libraries(main Qt6::Widgets)

