cmake_minimum_required(VERSION 3.10.0)
project(pract_JULY VERSION 0.1.0 LANGUAGES C CXX)
add_executable(pract_JULY main.cpp Life.cpp)


target_include_directories(pract_JULY PRIVATE ${raylib_INCLUDE_DIRS})

target_link_libraries(pract_JULY PRIVATE raylib)
