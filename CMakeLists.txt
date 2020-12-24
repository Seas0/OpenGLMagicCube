# project info
project(cube)
cmake_minimum_required(VERSION 3.1)

# OpenGL policy settting
if (POLICY CMP0072)
    cmake_policy (SET CMP0072 NEW)
endif(POLICY CMP0072)

# find requirements
find_package(Freetype REQUIRED)
if(NOT FREETYPE_FOUND)
    message("ERROR: Freetype not found")
endif(NOT FREETYPE_FOUND)
find_package(OpenGL REQUIRED)
if(NOT OPENGL_FOUND)
    message("ERROR: OpenGL not found")
endif(NOT OPENGL_FOUND)
set(GL_LIBRARY GL X11)

# shader
configure_file(resource/shader/fragmentShader.glsl resource/shader/fragmentShader.glsl COPYONLY)
configure_file(resource/shader/vertexShader.glsl resource/shader/vertexShader.glsl COPYONLY)
configure_file(resource/shader/fragmentShaderColor.glsl resource/shader/fragmentShaderColor.glsl COPYONLY)
configure_file(resource/shader/vertexShaderColor.glsl resource/shader/vertexShaderColor.glsl COPYONLY)
configure_file(resource/shader/fragmentShaderColor.glsl resource/shader/fragmentShaderText.glsl COPYONLY)
configure_file(resource/shader/vertexShaderColor.glsl resource/shader/vertexShaderText.glsl COPYONLY)

# texture
configure_file(resource/texture/Back.1536x1536.png resource/texture/cube00.png COPYONLY)
configure_file(resource/texture/Seas0.1536x1536.png resource/texture/cube01.png COPYONLY)
configure_file(resource/texture/Left.1536x1536.png resource/texture/cube02.png COPYONLY)
configure_file(resource/texture/Right.1536x1536.png resource/texture/cube03.png COPYONLY)
configure_file(resource/texture/Bottom.1536x1536.png resource/texture/cube04.png COPYONLY)
configure_file(resource/texture/Top.1536x1536.png resource/texture/cube05.png COPYONLY)
# configure_file(resource/texture/awesomeface.png resource/texture/awesomeface.png COPYONLY)
# configure_file(resource/texture/wall.jpg resource/texture/wall.jpg COPYONLY)

# font
configure_file(resource/font/simhei.ttf resource/font/simhei.ttf COPYONLY)

# include dir & source file & library
include_directories(include/common)
add_executable(cube main.cpp source/glad.c)
target_link_libraries(cube glfw GL m Xrandr Xi X11 Xxf86vm pthread dl Xinerama Xcursor freetype)
