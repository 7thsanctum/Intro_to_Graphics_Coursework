#include "post_process.h"
#include "geometry.h"

geometry* geom = 0;

glm::vec3 screen_coords[4] = 
{
	glm::vec3(-1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, 1.0f, 0.0f),
	glm::vec3(1.0f, -1.0f, 0.0f),
	glm::vec3(-1.0f, -1.0f, 0.0f)
};

glm::vec2 screen_tex_coords[4] = 
{
	glm::vec2(0.0f, 1.0f),
	glm::vec2(1.0f, 1.0f),
	glm::vec2(1.0f, 0.0f),
	glm::vec2(0.0f, 0.0f)
};

post_process::post_process(effect* effect)
	: _effect(effect), _framebuffer(0)
{
}

post_process::~post_process()
{
	if(_framebuffer)
		delete _framebuffer;
}

bool post_process::create(GLuint width, GLuint height)
{
	if (!geom)
	{
		geom = new geometry();
		for (int i = 0; i < 4; ++i)
		{
			geom->vertices.push_back(screen_coords[i]);
			geom->texcoords.push_back(screen_tex_coords[i]);
		}
		initialiseBuffers(geom);
	}
	_framebuffer = new frame_buffer(width, height);
	return _framebuffer->create();
}

void post_process::beginRender(bool clear)
{
	_framebuffer->bind();
	if (clear)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void post_process::endRender()
{
	_framebuffer->unbind();
}

void post_process::render(bool clear)
{
	if (clear)
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	_effect->begin();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _framebuffer->getTextureID());
	glUniform1i(_effect->getUniformIndex("tex"), 0);

	glBindVertexArray(geom->vao);
	glDrawArrays(GL_QUADS, 0, 4);
	glBindVertexArray(0);

	_effect->end();
}