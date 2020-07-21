#install (TARGETS ${PROJECT_NAME} DESTINATION bin)

#set(ASSET_DIR ${CMAKE_CURRENT_SOURCE_DIR}/assets)
#install (DIRECTORY ${ASSET_DIR} DESTINATION bin)

#install (FILES D:/Projects/Intro_to_Graphics_Coursework/Libraries/glfw-2.7.6.bin.WIN32/lib-msvc100/GLFW.dll DESTINATION bin/)
#install (FILES D:/Projects/Intro_to_Graphics_Coursework/Libraries/glew-1.9.0/bin/glew32.dll DESTINATION bin/)
cmake_minimum_required(VERSION 3.11)
# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
endif(MSVC)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(GFX_Submission)
#### Setup Directories ####
#Main output directory
SET (CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")
SET (CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
SET (CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
# Ouput all DLLs from all libs into main build folder

#### Add External Dependencies ####
#enu_graphics_framework
add_subdirectory("set08116_framework")

#Glew is dumb and won't listen to our build directories, so manually set it.
set_target_properties(libglew_shared PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
  ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}
  LIBRARY_OUTPUT_DIRECTORY ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}
)

add_executable(${PROJECT_NAME}
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

# TODO: Remove Boost dependency, this is currently required for JSON parsing.
# TODO: Upgrade GLM to a newer version.
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
"${CMAKE_CURRENT_SOURCE_DIR}/set08116_framework/lib/glfw/include"
"${CMAKE_CURRENT_SOURCE_DIR}/set08116_framework/lib/glew/include"
"${CMAKE_CURRENT_SOURCE_DIR}/set08116_framework/lib/"
# "${CMAKE_CURRENT_SOURCE_DIR}/set08116_framework/lib/glm/"
"${CMAKE_CURRENT_SOURCE_DIR}/../Libraries/glm-0.9.3.4"
)

#dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE enu_graphics_framework)

#copy General resources to build post build script
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD  
COMMAND ${CMAKE_COMMAND} -E copy_directory  "${PROJECT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")

set_target_properties(enu_graphics_framework PROPERTIES FOLDER "DEPS")
