# Project definition
#cmake_minimum_required(VERSION 3.11)
#project(GFX_Submission)

#add_subdirectory("set08116_framework")

# Source files
#set(SOURCES
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/main.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/util.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/texture.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/skybox.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/scene.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/post_process.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/point_sprite.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/geometry.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/frame_buffer.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/effect.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/cubemap.cpp"
#	"${CMAKE_CURRENT_SOURCE_DIR}/source/camera.cpp")

# Executable definition and properties
#add_executable(${PROJECT_NAME} ${SOURCES})
#target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
#"D:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/include"
#"D:/Projects/Intro_to_Graphics_Coursework/Libraries/glm-0.9.3.4"
#"D:/Projects/Intro_to_Graphics_Coursework/Libraries/glew-1.9.0/include"
#"D:/Projects/Intro_to_Graphics_Coursework/Libraries/glfw-2.7.6.bin.WIN32/include"
#"D:/Projects/Intro_to_Graphics_Coursework/Libraries/boost_1_51")

#set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 11)

#target_link_libraries(${PROJECT_NAME}
#	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/glfw-2.7.6.bin.WIN32/lib-msvc100/GLFW.lib
#	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/glfw-2.7.6.bin.WIN32/lib-msvc100/GLFWDLL.lib
#	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/glew-1.9.0/lib/glew32.lib	
#	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/DevIL.lib
#	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/ILUT.lib
#	PUBLIC D:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/ILU.lib
#)


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

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include"
"${CMAKE_CURRENT_SOURCE_DIR}/set08116_framework/lib/glfw/include"
"${CMAKE_CURRENT_SOURCE_DIR}/set08116_framework/lib/glew/include"
"${CMAKE_CURRENT_SOURCE_DIR}/set08116_framework/lib/"
# "${CMAKE_CURRENT_SOURCE_DIR}/set08116_framework/lib/glm/"
"E:/Projects/Intro_to_Graphics_Coursework/Libraries/glm-0.9.3.4"
# "E:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/include"
"E:/Projects/Intro_to_Graphics_Coursework/Libraries/boost_1_51"
)

#dependencies
target_link_libraries(${PROJECT_NAME} PRIVATE enu_graphics_framework
PUBLIC E:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/DevIL.lib
PUBLIC E:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/ILUT.lib
PUBLIC E:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/ILU.lib)
#add_dependencies(${childName} enu_graphics_framework )

#copy General resources to build post build script
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD  
COMMAND ${CMAKE_COMMAND} -E copy_directory  "${PROJECT_SOURCE_DIR}/assets" "$<TARGET_FILE_DIR:${PROJECT_NAME}>/assets")

#Grab practical folders
# file(GLOB children RELATIVE "${CMAKE_CURRENT_SOURCE_DIR}" practicals/*)
# SET(dirlist "")
# FOREACH(child ${children})
#   IF(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${child})
#     get_filename_component(childName ${child} NAME BASE_DIR practicals)
#     LIST(APPEND dirlist ${childName})
#     file(GLOB_RECURSE SOURCE_FILES ${child}/*.cpp ${child}/*.h ${child}/*.frag ${child}/*.vert ${child}/*.geom ${child}/*.comp)
#     file(GLOB_RECURSE NOT_RESOURCE_FILES ${child}/*.cpp ${child}/*.h)
#     file(GLOB_RECURSE RESOURCE_FILES  ${child}/*.*)
#     LIST(REMOVE_ITEM RESOURCE_FILES ${NOT_RESOURCE_FILES}) 
    	
    #add_custom_command(TARGET ${childName} POST_BUILD
		#	COMMAND ${CMAKE_COMMAND} -E copy_directory
		#	"${PROJECT_SOURCE_DIR}/set08116_framework/res"
		#	$<TARGET_FILE_DIR:${childName}>
	  #)
	
    # if(RESOURCE_FILES)
    # message("Resources: ${RESOURCE_FILES}")
    #   #copy Project Specific Resources
    #   FOREACH(res ${RESOURCE_FILES})
    #     get_filename_component(fn ${res} NAME)
    #     add_custom_command(TARGET ${childName} POST_BUILD  
    #     COMMAND ${CMAKE_COMMAND} -E copy_if_different ${res} $<TARGET_FILE_DIR:${childName}>/${childName}/${fn})
    #   ENDFOREACH()
    # endif(RESOURCE_FILES)
    
    #set outDir as debugging directory for each project
    # if(${MSVC})
    # file( WRITE "${CMAKE_CURRENT_BINARY_DIR}/${childName}.vcxproj.user" 
    #     "<?xml version=\"1.0\" encoding=\"utf-8\"?>
    #     <Project ToolsVersion=\"4.0\" xmlns=\"http://schemas.microsoft.com/developer/msbuild/2003\">
    #     <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Debug|x64'\">
    #         <LocalDebuggerWorkingDirectory>$(TargetDir)</LocalDebuggerWorkingDirectory>
    #         <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
    #     </PropertyGroup>
    #     <PropertyGroup Condition=\"'$(Configuration)|$(Platform)'=='Release|x64'\">
    #         <LocalDebuggerWorkingDirectory>$(TargetDir)</LocalDebuggerWorkingDirectory>
    #         <DebuggerFlavor>WindowsLocalDebugger</DebuggerFlavor>
    #     </PropertyGroup>
    #     </Project>")
    # endif()
#   ENDIF()
# ENDFOREACH()

set_target_properties(enu_graphics_framework PROPERTIES FOLDER "DEPS")


install (FILES E:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/DevIL.dll DESTINATION bin)
install (FILES E:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/ILUT.dll DESTINATION bin)
install (FILES E:/Projects/Intro_to_Graphics_Coursework/Libraries/DevIL-SDK-x86-1.7.8/lib/ILU.dll DESTINATION bin)