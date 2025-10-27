cmake_minimum_required(VERSION 3.10)

project(project)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL3 REQUIRED)

add_executable(
  project
  cpp.cpp
)

target_link_libraries(project PRIVATE
  SDL3::SDL3
)