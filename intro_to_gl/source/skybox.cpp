#include "skybox.h"
#include <glm\gtc\type_ptr.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include "geometry.h"
#include "effect.h"
#include "util.h"

geometry* skybox_geom = 0;

effect skybox_effect; 

skybox::skybox(cubemap* cubemap)
	: _cubemap(cubemap)
{
}

skybox::~skybox()
{
	delete _cubemap;
}

bool skybox::create()
{
	if (!skybox_geom)
	{
		skybox_geom = createBox();

		if (!skybox_effect.addShader("assets/shaders/skybox.vert", GL_VERTEX_SHADER))
			exit(EXIT_FAILURE);
		if (!skybox_effect.addShader("assets/shaders/skybox.frag", GL_FRAGMENT_SHADER))
			exit(EXIT_FAILURE);
		if (!skybox_effect.create())
			exit(EXIT_FAILURE);
	}

	return true;
}

void skybox::render(const camera* camera)
{
	CHECK_GL_ERROR
	glm::mat4 model = glm::translate(glm::mat4(1.0f), camera->getPosition());
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	glm::mat4 mvp = camera->getProjection() * camera->getView() * model;

	glDisable(GL_DEPTH_TEST);
	glDepthMask(false);

	skybox_effect.begin();
	glUniformMatrix4fv(skybox_effect.getUniformIndex("modelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));
	CHECK_GL_ERROR

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, _cubemap->getImageID());
	glUniform1i(skybox_effect.getUniformIndex("cubemap"), 0);
	CHECK_GL_ERROR

	glBindVertexArray(skybox_geom->vao);
	CHECK_GL_ERROR
	glDrawArrays(GL_TRIANGLES, 0, skybox_geom->vertices.size());
	CHECK_GL_ERROR
	glBindVertexArray(0);
	CHECK_GL_ERROR

	glDepthMask(true);
	glEnable(GL_DEPTH_TEST);
	skybox_effect.end();
	CHECK_GL_ERROR
}