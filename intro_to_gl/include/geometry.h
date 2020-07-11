#pragma once
#include "texture.h"
#include <vector>
#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>

struct geometry
{
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::vector<glm::vec2> texcoords;
	std::vector<glm::vec4> colours;
	std::vector<unsigned int> indices;

	GLuint vao;
	GLuint vertexBuffer;
	GLuint normalBuffer;
	GLuint texcoordBuffer;
	GLuint colourBuffer;
	GLuint indexBuffer;

	geometry()
		: vertexBuffer(0), normalBuffer(0), vao(0), texcoordBuffer(0), colourBuffer(0), indexBuffer(0)
	{
	}

	~geometry()
	{
		if (vertexBuffer)
			glDeleteBuffers(1, &vertexBuffer);
		if (normalBuffer)
			glDeleteBuffers(1, &normalBuffer);
		if (texcoordBuffer)
			glDeleteBuffers(1, &texcoordBuffer);
		if (colourBuffer)
			glDeleteBuffers(1, &colourBuffer);
		if (indexBuffer)
			glDeleteBuffers(1, &indexBuffer);
		if (vao)
			glDeleteVertexArrays(1, &vao);
	}
};

geometry *createBox();

geometry *createTetrahedron();

geometry *createPyramid();

geometry *createDisk(int slices);

geometry *createCylinder(int stacks, int slices);

geometry *createSphere(int stacks, int slices);

geometry *createSphereTri(int divisions);

geometry *createTorus(float radius, int stacks, int slices);

geometry *createPlane(int width, int depth);

geometry *createSierpinski(int divisions);

geometry *createTerrain(texture *tex);

void initialiseBuffers(geometry *geom);
