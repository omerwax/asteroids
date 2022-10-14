cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(SpaceInvaders)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
message("SDL2_INCLUDE PATH = " ${SDL2_INCLUDE_DIRS})
include_directories(${SDL2_INCLUDE_DIRS} src)

file(GLOB src_files "src/*.cpp")

#print (${src_files})

add_executable(space-invaders ${src_files})
string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(space-invaders ${SDL2_LIBRARIES})
