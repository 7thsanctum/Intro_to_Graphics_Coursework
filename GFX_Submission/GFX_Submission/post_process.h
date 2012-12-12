#pragma once

#include "frame_buffer.h"
#include "effect.h"

class post_process
{
private:
	frame_buffer* _framebuffer;
	effect* _effect;
public:
	post_process(effect* effect);
	~post_process();

	bool create(GLuint width, GLuint height);

	effect* getEffect() { return _effect; }
	void setEffect(effect* effect) { _effect = effect; }
	frame_buffer* getFrameBuffer() { return _framebuffer; }

	void beginRender(bool clear);

	void endRender();

	void render(bool clear);
};