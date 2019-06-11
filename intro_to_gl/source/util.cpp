#include "util.h"
#include <iostream>
#include <fstream>

void setDebug()
{
#if defined(DEBUG) | defined(_DEBUG)
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
}

void getGLError(int line, std::string file)
{
	GLenum error = glGetError();
	if (error)
		std::cout << "OpenGL error: " << gluErrorString(error)
				  << " at line " << line << " in file "
				  << file.c_str() << std::endl;
}