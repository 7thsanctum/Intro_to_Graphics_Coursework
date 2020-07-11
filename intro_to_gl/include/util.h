#pragma once

#include <GLFW\glfw3.h>
#include <string>
#pragma comment(lib, "Glu32")

#if defined(DEBUG) || defined(_DEBUG)
#define CHECK_GL_ERROR getGLError(__LINE__, __FILE__);
#define SET_DEBUG setDebug();
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#else
#define CHECK_GL_ERROR
#define SET_DEBUG
#endif

void setDebug();

void getGLError(int line, std::string file);