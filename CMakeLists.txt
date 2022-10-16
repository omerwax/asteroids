cmake_minimum_required(VERSION 3.7)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(Asteroids)

# Add SDL2 CMake modules
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS} src)

file(GLOB src_files "src/*.cpp")

# Add SDL2_ttf library
find_package(SDL2_ttf REQUIRED)


#print (${src_files})

add_executable(asteroids ${src_files})
# string(STRIP ${SDL2_LIBRARIES} SDL2_LIBRARIES)
target_link_libraries(asteroids ${SDL2_LIBRARIES})
target_link_libraries(asteroids SDL2::TTF) 