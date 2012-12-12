#pragma once 

#include <GL/glew.h>

class frame_buffer
{
private :
	GLuint _framebuffer;

	GLuint _texture;

	GLuint _depth;

	GLuint _width;

	GLuint _height;

public:
	frame_buffer(GLuint width, GLuint height);
	~frame_buffer();
	GLuint getTextureID() { return _texture; }
	GLuint getDepthBuffer() { return _depth; }

	bool create();

	void bind();
	void unbind();
};