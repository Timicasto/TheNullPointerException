cmake_minimum_required(VERSION 3.21)
project(OpenGLC__)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 23)

include_directories("include")
include_directories("glm")
link_directories(lib-mingw-w64)

add_executable(OpenGLC__ main.cpp glad.c font.cpp font.h Shader.cpp Shader.h)

target_link_libraries(OpenGLC__ libglfw3dll.a libglfw3.a glew32.lib opengl32 freetype.lib)

