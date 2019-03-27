# Specify the minimum version for CMake

cmake_minimum_required(VERSION 3.7)

# Project's name

project(kkrworlds)

# Set the output folder where your program will be created

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)
#set(opengl_PATH "opengl path" CACHE STRING "")
#SET(opengl_IMAGELIB "${opengl_PATH}/lib/libopengl_image.dll.a")
# The following folder will be included

include_directories("${PROJECT_SOURCE_DIR}")



add_executable(kkrworlds ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/shader.cpp)

target_link_libraries(kkrworlds ${PROJECT_SOURCE_DIR}/lib/libglfw3dll.a opengl32 ${PROJECT_SOURCE_DIR}/lib/glew32.lib)