#include "point_sprite.h"
#include "util.h"
#include <glm\gtc\type_ptr.hpp>

/*
Creates the point sprite object. All we need to do here is set the geomtry and texture data
*/
point_sprite::point_sprite(geometry* geom, texture* tex)
	: _geom(geom), _tex(tex)
{
}

/*
Destroys the point sprite. Really we don't have to do anything here
*/
point_sprite::~point_sprite()
{
}

/*
Enables some required values for the point sprites. This only
really needs to be done once.
*/
bool point_sprite::create()
{
	// enable blending and the blend function type
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	
	glDisable(GL_BLEND);
	// Enable point sprites and the ability to set point sizes in the
	// shader programs
	glEnable(GL_POINT_SPRITE);
	glEnable(GL_PROGRAM_POINT_SIZE);
	
	CHECK_GL_ERROR

	return true;
}

/*
Renders the point sprites to the screen
*/
void point_sprite::render(effect* eff, const camera* camera)
{
	// We need to model view projection for the point sprite, and apply to effect
	glm::mat4 mvp = camera->getProjection() * camera->getView() * transform.getTransformMatrix();
	glUniformMatrix4fv(eff->getUniformIndex("modelViewProjection"), 1, GL_FALSE, glm::value_ptr(mvp));
	CHECK_GL_ERROR

	// Set the texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, _tex->getImageID());
	glUniform1i(eff->getUniformIndex("tex"), 0);
	CHECK_GL_ERROR

	// Bind the vertex array
	glBindVertexArray(_geom->vao);

	// Draw the vertex array as points
	glDrawArrays(GL_POINTS, 0, _geom->vertices.size());
	CHECK_GL_ERROR
	
	glBindVertexArray(0);
}

/*
Helper function used to generate a star field set of points
*/

geometry* createStarField(int num_stars)
{
	// Create the geomtery object
	geometry* geom = new geometry();
	
	// Colours we will use for the starfield
	glm::vec4 colours[4] = 
	{
		glm::vec4(1.0f, 1.0f, 1.0f, 1.0f),
		glm::vec4(0.67f, 0.68f, 0.82f, 1.0f),
		glm::vec4(1.0f, 0.5f, 0.5f, 1.0f),
		glm::vec4(1.0f, 0.82f, 0.65f, 1.0f)
	};

	// Create the number of stars required
	for (int i = 0; i < num_stars; ++i)
	{
		// Randomly select the colour
		int col = 0;
		if (rand() % 5 == 0)
			col = 1;
		if (rand() % 50 == 0)
			col = 2;
		if (rand() % 500 == 0)
			col = 3;

		// Push back the colour
		geom->colours.push_back(colours[col]);

		// Randomly select a position
		glm::vec3 position = glm::vec3(
			float(3000.0f - (rand() % 6000)) * 0.1f,
			float(3000.0f - (rand() % 6000)) * 0.1f,
			-float(rand() % 1000) - 1.0f);

		// Push back the position 
		geom->vertices.push_back(position);
	}

	// Initialise the geometry buffers
	initialiseBuffers(geom);

	return geom;
}
