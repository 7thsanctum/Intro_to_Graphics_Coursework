# Project definition
cmake_minimum_required(VERSION 3.11)
project(GFX_Submission)

add_subdirectory("set08116_framework")

# Source files
set(SOURCES 
	"${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/util.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/texture.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/skybox.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/scene.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/post_process.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/point_sprite.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/geometry.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/frame_buffer.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/effect.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/cubemap.cpp"
	"${CMAKE_CURRENT_SOURCE_DIR}/source/camera.cpp")

# Executable definition and properties
add_executable(${PROJECT_NAME} ${SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
"D:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/include"
"D:/Projects/Intro_to_Graphics_Coursework/Libraries/glm-0.9.3.4"
"D:/Projects/Intro_to_Graphics_Coursework/Libraries/glew-1.9.0/include"
"D:/Projects/Intro_to_Graphics_Coursework/Libraries/glfw-2.7.6.bin.WIN32/include"
"D:/Projects/Intro_to_Graphics_Coursework/Libraries/boost_1_51")

set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

target_link_libraries(${PROJECT_NAME}
	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/glfw-2.7.6.bin.WIN32/lib-msvc100/GLFW.lib
	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/glfw-2.7.6.bin.WIN32/lib-msvc100/GLFWDLL.lib
	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/glew-1.9.0/lib/glew32.lib	
	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/DevIL.lib
	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/ILUT.lib
	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/ILU.lib
)


install (TARGETS ${PROJECT_NAME} DESTINATION bin)

set(ASSET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
install (DIRECTORY ${ASSET_DIR} DESTINATION bin)

install (FILES D:/Projects/Intro_to_Graphics_Coursework/Libraries/glfw-2.7.6.bin.WIN32/lib-msvc100/GLFW.dll DESTINATION bin/)
install (FILES D:/Projects/Intro_to_Graphics_Coursework/Libraries/glew-1.9.0/bin/glew32.dll DESTINATION bin/)
install (FILES D:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/DevIL.dll DESTINATION bin/)
install (FILES D:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/ILUT.dll DESTINATION bin/)
install (FILES D:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/ILU.dll DESTINATION bin/)
