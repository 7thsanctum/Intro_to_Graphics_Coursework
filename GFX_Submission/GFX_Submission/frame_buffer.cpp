#include "frame_buffer.h"
#include "util.h"
#include <cstdio>

frame_buffer::frame_buffer(GLuint width, GLuint height)
	: _framebuffer(0), _texture(0), _depth(0), _width(width), _height(height)
{
}

frame_buffer::~frame_buffer()
{
	if (_depth) glDeleteBuffers(1, &_depth);
	if (_texture) glDeleteBuffers(1, &_texture);
	if (_framebuffer) glDeleteBuffers(1, &_framebuffer);
}

bool frame_buffer::create()
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &_texture);
	glBindTexture(GL_TEXTURE_2D, _texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, _width, _height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	CHECK_GL_ERROR

	glGenRenderbuffers(1, &_depth);
	glBindRenderbuffer(GL_RENDERBUFFER, _depth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, _width, _height);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	CHECK_GL_ERROR

	glGenFramebuffers(1, &_framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _texture, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, _depth);
	CHECK_GL_ERROR

	GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("failed to create framebuffer: error %p\n", status);
		return false;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return true;
}

void frame_buffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, _framebuffer);
}

void frame_buffer::unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}