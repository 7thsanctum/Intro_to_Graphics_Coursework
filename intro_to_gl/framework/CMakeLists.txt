# Reduced version of https://github.com/edinburgh-napier/set08116_framework
# Primarily for the submodules

cmake_minimum_required(VERSION 3.11)
# Compiler flags
if (MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP /EHsc" CACHE INTERNAL "" FORCE)
endif(MSVC)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(framework)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.h)
add_library(framework STATIC ${SOURCE_FILES})
target_include_directories(framework PUBLIC src)
	
#GLFW options
option(GLFW_BUILD_DOCS "" OFF)
option(GLFW_BUILD_EXAMPLES "" OFF)
option(GLFW_BUILD_TESTS "" OFF)
option(GLFW_DOCUMENT_INTERNALS "" OFF)
option(GLFW_INSTALL "" OFF)
#GLEW options
option(BUILD_UTILS "" OFF)
option(BUILD_SHARED_LIBS "" ON)
option(glew-cmake_BUILD_MULTI_CONTEXT "" OFF)
option(glew-cmake_BUILD_SHARED "" ON)
option(glew-cmake_BUILD_SINGLE_CONTEXT "" ON)
option(glew-cmake_BUILD_STATIC "" OFF)
#GLM options
option(GLM_DYNAMIC_LIBRARY_ENABLE "" OFF)
option(GLM_INSTALL_ENABLE "" OFF)
option(GLM_TEST_ENABLE "" OFF)

set(BuildTests OFF CACHE BOOL "" FORCE)

#================================================
#### Add External Dependencies ####
#================================================

# GLFW https://github.com/glfw/glfw.git 56ecd62f
add_subdirectory("lib/glfw")
target_include_directories(framework PUBLIC "lib/glfw/include/")
#================================================
# GLEW https://github.com/Perlmint/glew-cmake.git ea68a21
add_subdirectory("lib/glew")
target_include_directories(framework PUBLIC "lib/glew/include")
#================================================
# GLM https://github.com/g-truc/glm.git 77332664
add_subdirectory("lib/glm")
target_include_directories(framework PUBLIC "lib/glm")
#================================================
add_library(stb_image INTERFACE)
target_include_directories(stb_image INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}/lib)
target_sources(stb_image INTERFACE 
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image.h
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/stb_image_write.h
)
#================================================
# OpenGL
FIND_PACKAGE( OpenGL REQUIRED)
if(NOT ${OPENGL_FOUND})
  message(FATAL_ERROR "OPENGL NOT FOUND")
endif()
#====================================================================
target_link_libraries(framework 
	PUBLIC ${OPENGL_gl_LIBRARY} 
	PUBLIC glfw
	PUBLIC libglew_shared
  PRIVATE stb_image
)
#====================================================================
