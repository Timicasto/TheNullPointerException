cmake_minimum_required(VERSION 3.19)
project(OpenGLC__)

set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD 23)

include_directories("include")
include_directories("glm")
link_directories(lib-mingw-w64)

add_executable(OpenGLC__ main.cpp glad.c font.cpp font.h Shader.cpp Shader.h fields.h Profile.cpp Profile.h)

target_link_libraries(OpenGLC__ glfw3 ${CMAKE_DL_LIBS} GLEW GL freetype assimp)

