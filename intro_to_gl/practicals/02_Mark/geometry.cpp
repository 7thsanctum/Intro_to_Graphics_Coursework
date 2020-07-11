#include "geometry.h"
#include "texture.h"
#include <glm\gtx\constants.hpp>
#include <glm\glm.hpp>
#include <GL\wglew.h>
#include "util.h"

void initialiseBuffers(geometry* geom)
{
	// Generate and bind the vertex array object
	glGenVertexArrays(1, &geom->vao);
	glBindVertexArray(geom->vao);

	// If we have vertices, then add to the vertex array object
	if (geom->vertices.size() > 0)
	{
		glGenBuffers(1, &geom->vertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, geom->vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, geom->vertices.size() * sizeof(glm::vec3), &geom->vertices[0], GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(0);
		CHECK_GL_ERROR
	}

	if (geom->normals.size() > 0)
	{
		glGenBuffers(1, &geom->normalBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, geom->normalBuffer);
		glBufferData(GL_ARRAY_BUFFER, geom->normals.size() * sizeof(glm::vec3), &geom->normals[0], GL_STATIC_DRAW);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(1);
		CHECK_GL_ERROR
	}

	if (geom->texcoords.size() > 0)
	{
		glGenBuffers(1, &geom->texcoordBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, geom->texcoordBuffer);
		glBufferData(GL_ARRAY_BUFFER, geom->texcoords.size() * sizeof(glm::vec2), &geom->texcoords[0], GL_STATIC_DRAW);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(2);
		CHECK_GL_ERROR
	}

	if (geom->colours.size() > 0)
	{
		glGenBuffers(1, &geom->colourBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, geom->colourBuffer);
		glBufferData(GL_ARRAY_BUFFER, geom->colours.size() * sizeof(glm::vec4), &geom->colours[0], GL_STATIC_DRAW);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, 0);
		glEnableVertexAttribArray(3);
		CHECK_GL_ERROR
	}
	glBindVertexArray(0);
}
//##########################################BOX#################################################################
glm::vec3 box_vertices[8] =
{
	glm::vec3(-1.0f, 1.0f, 1.0f),		// 0
	glm::vec3(1.0f, 1.0f, 1.0f),		// 1
	glm::vec3(1.0f, -1.0f, 1.0f),		// 2
	glm::vec3(-1.0f, -1.0f, 1.0f),		// 3
	glm::vec3(1.0f, 1.0f, -1.0f),		// 4
	glm::vec3(-1.0f, 1.0f, -1.0f),		// 5
	glm::vec3(-1.0f, -1.0f, -1.0f),		// 6
	glm::vec3(1.0f, -1.0f, -1.0f)		// 7
};

glm::vec2 box_texcoords[6] =
{
	glm::vec2(0.0f, 1.0f),		// 0
	glm::vec2(1.0f, 1.0f),		// 1
	glm::vec2(0.0f, 0.0f),		// 3
	glm::vec2(1.0f, 1.0f),		// 5
	glm::vec2(1.0f, 0.0f),		// 6
	glm::vec2(0.0f, 0.0f)		// 7
};

glm::vec3 box_normals[8] =
{
	glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f)),
	glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),
	glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)),
	glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)),
	glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)),
	glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)),
	glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
	glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f))
};

unsigned int box_indices[36] = 
{
	// Side 1
	0, 1, 3,
	1, 2, 3,
	//Side 2
	1, 4, 2,
	4, 7, 2,
	// Side 3
	4, 5, 7,
	5, 6, 7,
	// Side 4
	5, 0, 6,
	0, 3, 6,
	// Side 5
	5, 4, 0,
	4, 1, 0,
	// Side 6
	3, 2, 6,
	2, 7, 6
};

geometry* createBox()
{
	geometry* geom = new geometry();
	for (int i = 0; i < 36; ++i)
	{
		geom->vertices.push_back(box_vertices[box_indices[i]]);
		geom->normals.push_back(box_normals[box_indices[i]]);
		geom->texcoords.push_back(box_texcoords[i % 6]);
	}
	
	initialiseBuffers(geom);

	return geom;
}

//##############################################################################################################
//##########################################TETRAHEDRON#########################################################
glm::vec3 tetrahedron_vertices[4] =
{
	glm::vec3(0.0f, 1.0f, 0.0f),		// 0
	glm::vec3(-1.0f, -1.0f, 1.0f),		// 1
	glm::vec3(1.0f, -1.0f, 1.0f),		// 2
	glm::vec3(0.0f, -1.0f, -1.0f),		// 3
};

glm::vec3 tetrahedron_normals[4] =
{
	glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
	glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)),
	glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)),
	glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f))
};

glm::vec2 tetrahedron_texcoords[3] = 
{
		glm::vec2(0.5f, 1.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(0.0f, 0.0f)
};

unsigned int tetrahedron_indices[12] = 
{
	// Side 1
	0, 2, 1,
	//Side 2
	0, 3, 2,
	// Side 3
	0, 1, 3,
	// Bottom
	1, 2, 3,
};

geometry* createTetrahedron()
{
	geometry* geom = new geometry();
	for (int i = 0; i < 12; ++i)
	{
		geom->vertices.push_back(tetrahedron_vertices[tetrahedron_indices[i]]);
		geom->normals.push_back(tetrahedron_normals[tetrahedron_indices[i]]);
		geom->texcoords.push_back(tetrahedron_texcoords[i % 3]);
	}

	initialiseBuffers(geom);
	
	return geom;
}
//##############################################################################################################
//#########################################PYRAMID##############################################################
glm::vec3 pyramid_vertices[5] =
{
	glm::vec3(0.0f, 1.0f, 0.0f),		// 0
	glm::vec3(-1.0f, -1.0f, 1.0f),		// 1
	glm::vec3(1.0f, -1.0f, 1.0f),		// 2
	glm::vec3(1.0f, -1.0f, -1.0f),		// 3
	glm::vec3(-1.0f, -1.0f, -1.0f)		// 4
};

glm::vec3 pyramid_normals[5] = 
{
	glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)),
	glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)),
	glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)),
	glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)),
	glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)),
};

glm::vec2 pyramid_texcoords[18] =
{
	// Face 1
	glm::vec2(0.5f, 1.0f),
	glm::vec2(1.0f, 0.0f),
	glm::vec2(0.0f, 0.0f),
	// Face 2
	glm::vec2(0.5f, 1.0f),
	glm::vec2(1.0f, 0.0f),
	glm::vec2(0.0f, 0.0f),
	// Face 3
	glm::vec2(0.5f, 1.0f),
	glm::vec2(1.0f, 0.0f),
	glm::vec2(0.0f, 0.0f),
	// Face 4
	glm::vec2(0.5f, 1.0f),
	glm::vec2(1.0f, 0.0f),
	glm::vec2(0.0f, 0.0f),

	// Bottom
	glm::vec2(0.0f, 1.0f),
	glm::vec2(1.0f, 0.0f),
	glm::vec2(0.0f, 0.0f),
	glm::vec2(0.0f, 1.0f),
	glm::vec2(1.0f, 1.0f),
	glm::vec2(1.0f, 0.0f)
};
unsigned int pyramid_indices[18] = 
{
	// Side 1
	0, 2, 1,
	//Side 2
	0, 3, 2,
	// Side 3
	0, 4, 3,
	// Side 4
	0, 1, 4,
	// Bottom
	1, 3, 4,
	1, 2, 3,
};

geometry* createPyramid()
{
	geometry* geom = new geometry();
	for (int i = 0; i < 18; ++i)
	{
		geom->vertices.push_back(pyramid_vertices[pyramid_indices[i]]);
		geom->normals.push_back(pyramid_normals[pyramid_indices[i]]);
		geom->texcoords.push_back(pyramid_texcoords[i]);
	}	

	initialiseBuffers(geom);

	return geom;
}
//##############################################################################################################
//########################################DISK##################################################################
geometry* createDisk(int slices)
{
	geometry* geom = new geometry();
	glm::vec3 centre(0.0f, 0.0f, 0.0);
	glm::vec3 vertex_prev(1.0f, 0.0f, 0.0);
	glm::vec3 vertex_current;
	float deltaAngle = (2.0f * glm::pi<float>()) / slices;
	for (int i = 1; i <= slices; ++i)
	{
		vertex_current = glm::vec3(cos(i * deltaAngle), 0.0f, sin(i * deltaAngle));
		geom->vertices.push_back(centre);
		geom->vertices.push_back(vertex_prev);
		geom->vertices.push_back(vertex_current);
		for (int j = 0; j < 3; ++j)
			geom->normals.push_back(glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
		geom->texcoords.push_back(glm::vec2(0.5f, 0.5f));
		geom->texcoords.push_back(glm::vec2(0.5f, 0.5f) + (glm::vec2(vertex_prev.x, vertex_prev.z) / 2.0f));
		geom->texcoords.push_back(glm::vec2(0.5f, 0.5f) + (glm::vec2(vertex_current.x, vertex_current.z) / 2.0f));
		vertex_prev = vertex_current;
	}
	
	initialiseBuffers(geom);
	
	return geom;
}
//##############################################################################################################
//########################################CYLINDER##############################################################
geometry* createCylinder(int stacks, int slices)
{
	geometry* geom = new geometry();
	// Create top
	glm::vec3 centre(0.0f, 1.0f, 0.0f);
	glm::vec3 vertex_prev(1.0f, 1.0f, 0.0f);
	glm::vec3 vertex_current;
	float deltaAngle = (2 * glm::pi<float>()) / slices;
	for (int i = 1; i <= slices; ++i)
	{
		vertex_current = glm::vec3(cos(i * deltaAngle), 1.0f, sin(i * deltaAngle));
		geom->vertices.push_back(centre);
		geom->normals.push_back(glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
		geom->vertices.push_back(vertex_prev);
		geom->normals.push_back(glm::normalize(glm::vec3(vertex_prev.x, 1.0f, vertex_prev.z)));
		geom->vertices.push_back(vertex_current);
		geom->normals.push_back(glm::normalize(glm::vec3(vertex_current.x, 1.0f, vertex_current.z)));
		geom->texcoords.push_back(glm::vec2(0.5f, 0.5f));
		geom->texcoords.push_back(glm::vec2(0.5f, 0.5f) + (glm::vec2(vertex_prev.x, vertex_prev.z) / 2.0f));
		geom->texcoords.push_back(glm::vec2(0.5f, 0.5f) + (glm::vec2(vertex_current.x, vertex_current.z) / 2.0f));
		vertex_prev = vertex_current;
	}

	//Create bottom
	centre = glm::vec3(0.0f, -1.0f, 0.0f);
	vertex_prev = glm::vec3(1.0f, -1.0f, 0.0f);
	for (int i = 1; i <= slices; ++i)
	{
		vertex_current = glm::vec3(cos(i * deltaAngle), -1.0f, sin(i * deltaAngle));
		geom->vertices.push_back(centre);
		geom->normals.push_back(glm::normalize(glm::vec3(0.0f, -1.0f, 0.0f)));
		geom->vertices.push_back(vertex_prev);
		geom->normals.push_back(glm::normalize(glm::vec3(vertex_prev.x, -1.0f, vertex_prev.z)));
		geom->vertices.push_back(vertex_current);
		geom->normals.push_back(glm::normalize(glm::vec3(vertex_current.x, -1.0f, vertex_current.z)));
		geom->texcoords.push_back(glm::vec2(0.5f, 0.5f));
		geom->texcoords.push_back(glm::vec2(0.5f, 0.5f) + (glm::vec2(vertex_prev.x, vertex_prev.z) / 2.0f));
		geom->texcoords.push_back(glm::vec2(0.5f, 0.5f) + (glm::vec2(vertex_current.x, vertex_current.z) / 2.0f));
		vertex_prev = vertex_current;
	}

	//Create stacks
	glm::vec3 vertices[4];
	float deltaHeight = 2.0f / stacks;	
	float deltaWidth = 1.0f / stacks;
	for (int i = 0; i < stacks; ++i)
	{
		for (int j = 0; j < slices; ++j)
		{
			vertices[0] = glm::vec3(cos(j * deltaAngle), 1.0f - (deltaHeight * i), sin(j * deltaAngle));
			vertices[1] = glm::vec3(cos((j + 1) * deltaAngle), 1.0f - (deltaHeight * i), sin((j + 1) * deltaAngle));
			vertices[2] = glm::vec3(cos(j * deltaAngle), 1.0f - (deltaHeight * (i + 1)), sin(j * deltaAngle));
			vertices[3] = glm::vec3(cos((j + 1) * deltaAngle), 1.0f - (deltaHeight * (i + 1)), sin((j + 1) * deltaAngle));
			// Triangle 1
			geom->vertices.push_back(vertices[0]);
			geom->normals.push_back(glm::normalize(glm::vec3(vertices[0].x, 0.0f, vertices[0].z)));
			geom->texcoords.push_back(glm::vec2(deltaWidth * j, 1.0f - ((deltaHeight * i) / 2)));
			geom->vertices.push_back(vertices[3]);
			geom->normals.push_back(glm::normalize(glm::vec3(vertices[3].x, 0.0f, vertices[3].z)));
			geom->texcoords.push_back(glm::vec2(deltaWidth * (j + 1), 1.0f - ((deltaHeight * (i + 1)) / 2)));
			geom->vertices.push_back(vertices[2]);
			geom->normals.push_back(glm::normalize(glm::vec3(vertices[2].x, 0.0f, vertices[2].z)));
			geom->texcoords.push_back(glm::vec2(deltaWidth * j, 1.0f - ((deltaHeight * (i + 1)) / 2)));
			// Triangle 2
			geom->vertices.push_back(vertices[0]);
			geom->normals.push_back(glm::normalize(glm::vec3(vertices[0].x, 0.0f, vertices[0].z)));
			geom->texcoords.push_back(glm::vec2(deltaWidth * j, 1.0f - ((deltaHeight * i) / 2)));
			geom->vertices.push_back(vertices[1]);
			geom->normals.push_back(glm::normalize(glm::vec3(vertices[1].x, 0.0f, vertices[1].z)));
			geom->texcoords.push_back(glm::vec2(deltaWidth * (j + 1), 1.0f - ((deltaHeight * i) / 2)));
			geom->vertices.push_back(vertices[3]);
			geom->normals.push_back(glm::normalize(glm::vec3(vertices[3].x, 0.0f, vertices[3].z)));
			geom->texcoords.push_back(glm::vec2(deltaWidth * (j + 1), 1.0f - ((deltaHeight * (i + 1)) / 2)));
		}
	}
	
	initialiseBuffers(geom);

	return geom;
}
//##############################################################################################################
//#############################################SPHERE###########################################################
geometry* createSphere(int stacks, int slices)
{
	geometry* geom = new geometry();
	// Create required values
	float deltaRho = glm::pi<float>() / slices;
	float deltaTheta = 2.0f * glm::pi<float>() / slices;
	float deltaT = 1.0f / (float)stacks;
	float deltaS = 1.0f / (float)slices;
	float t = 1.0f;
	float s = 0.0f;
	for (int i = 0; i < stacks; ++i)
	{
		float rho = i * deltaRho;
		s = 0.0f;
		glm::vec3 vertices[4];
		glm::vec2 texcoords[4];
		for (int j = 0; j < slices; ++j)
		{
			//Vertex 0
			float theta = j * deltaTheta;
			vertices[0] = glm::vec3(-sin(theta) * sin(rho), cos(theta) * sin(rho), cos(rho));
			texcoords[0] = glm::vec2(s, t);
			//Vertex 1
			vertices[1] = glm::vec3(-sin(theta) * sin(rho + deltaRho), cos(theta) * sin(rho + deltaRho), cos(rho + deltaRho));
			texcoords[1] = glm::vec2(s, t - deltaT);
			//Vertex 2
			theta = ((j + 1) == slices) ? 0.0f : (j + 1) * deltaTheta;
			s += deltaS;
			vertices[2] = glm::vec3(-sin(theta) * sin(rho), cos(theta) * sin(rho), cos(rho));
			texcoords[2] = glm::vec2(s, t);
			//Vertex 3
			vertices[3] = glm::vec3(-sin(theta) * sin(rho + deltaRho), cos(theta) * sin(rho + deltaRho), cos(rho + deltaRho));
			texcoords[3] = glm::vec2(s, t - deltaT);

			// Triangle 1
			geom->vertices.push_back(vertices[0]);
			geom->normals.push_back(glm::normalize(vertices[0]));
			geom->texcoords.push_back(texcoords[0]);
			geom->vertices.push_back(vertices[1]);
			geom->normals.push_back(glm::normalize(vertices[1]));
			geom->texcoords.push_back(texcoords[1]);
			geom->vertices.push_back(vertices[2]);
			geom->normals.push_back(glm::normalize(vertices[2]));
			geom->texcoords.push_back(texcoords[2]);
			// Triangle 2
			geom->vertices.push_back(vertices[1]);
			geom->normals.push_back(glm::normalize(vertices[1]));
			geom->texcoords.push_back(texcoords[1]);
			geom->vertices.push_back(vertices[3]);
			geom->normals.push_back(glm::normalize(vertices[3]));
			geom->texcoords.push_back(texcoords[3]);
			geom->vertices.push_back(vertices[2]);
			geom->normals.push_back(glm::normalize(vertices[2]));
			geom->texcoords.push_back(texcoords[2]);
		}
		t -= deltaT;
	}
	
	initialiseBuffers(geom);

	return geom;
}
//##############################################################################################################
//###############################################TORUS##########################################################
geometry* createTorus(float radius, int stacks, int slices)
{
	geometry* geom = new geometry();
	float deltaStack = 2.0f * glm::pi<float>() / stacks;
	float deltaSlice = 2.0f * glm::pi<float>() / slices;
	for (int i = 0; i < stacks; ++i)
	{
		float a0 = i * deltaStack;
		float a1 = a0 + deltaStack;
		glm::vec3 vertices[4];
		glm::vec3 normals[4];
		glm::vec2 texcoords[4];
		for (int j = 0; j <= slices; ++j)
		{
			float b = j * deltaSlice;
			float c = cos(j * deltaSlice);
			float r = c + radius;
			vertices[0] = glm::vec3(sin(a0) * r, sin(j * deltaSlice), cos(a0) * r);
			normals[0] = glm::vec3(sin(a0) * c, cos(a0) * c, sin(b));
			texcoords[0] = glm::vec2((float)i / stacks, (float)j / slices);
			vertices[1] = glm::vec3(sin(a1) * r, sin(j * deltaSlice), cos(a1) * r);
			normals[1] = glm::vec3(sin(a1) * c, cos(a1) * c, sin(b));
			texcoords[1] = glm::vec2((float)(i + 1) / stacks, (float)j / slices);
			c = cos((j + 1) * deltaSlice);
			r = c + radius;
			vertices[2] = glm::vec3(sin(a0) * r, sin((j + 1) * deltaSlice), cos(a0) * r);
			normals[2] = glm::vec3(sin(a0) * c, cos(a0) * c, sin(b));
			texcoords[2] = glm::vec2((float)i / stacks, (float)(j + 1) / slices);
			vertices[3] = glm::vec3(sin(a1) * r, sin((j + 1) * deltaSlice), cos(a1) * r);
			normals[3] = glm::vec3(sin(a1) * c, cos(a1) * c, sin(b));
			texcoords[3] = glm::vec2((float)(i + 1) / stacks, (float)(j + 1) / slices);
			// Triangle 1
			geom->vertices.push_back(vertices[0]);
			geom->normals.push_back(glm::normalize(normals[0]));
			geom->texcoords.push_back(texcoords[0]);
			geom->vertices.push_back(vertices[1]);
			geom->normals.push_back(glm::normalize(normals[1]));
			geom->texcoords.push_back(texcoords[1]);
			geom->vertices.push_back(vertices[2]);
			geom->normals.push_back(glm::normalize(normals[2]));
			geom->texcoords.push_back(texcoords[2]);
			// Triangle 2
			geom->vertices.push_back(vertices[1]);
			geom->normals.push_back(glm::normalize(normals[1]));
			geom->texcoords.push_back(texcoords[1]);
			geom->vertices.push_back(vertices[3]);
			geom->normals.push_back(glm::normalize(normals[3]));
			geom->texcoords.push_back(texcoords[3]);
			geom->vertices.push_back(vertices[2]);
			geom->normals.push_back(glm::normalize(normals[2]));
			geom->texcoords.push_back(texcoords[2]);
		}
	}
	
	initialiseBuffers(geom);

	return geom;
}
//##############################################################################################################
//###############################################PLANE##########################################################
geometry* createPlane(int width, int depth)
{
	geometry* geom = new geometry();
	glm::vec3 vertices[4];
	float deltaWidth = 1.0f / width;
	float deltaDepth= 1.0f / depth;		
	for (int x = 0; x < width; ++x)
	{
		for (int z = 0; z < depth; ++z)
		{			
			vertices[0] = glm::vec3(-((float)width / 2.0f) + x, 0.0f, -((float)depth / 2.0f) + z);
			vertices[1] = glm::vec3(-((float)width / 2.0f) + (x + 1), 0.0f, -((float)depth / 2.0f) + z);
			vertices[2] = glm::vec3(-((float)width / 2.0f) + x, 0.0f, -((float)depth / 2.0f) + (z + 1));
			vertices[3] = glm::vec3(-((float)width / 2.0f) + (x+1), 0.0f, -((float)depth / 2.0f) + (z + 1));
			// Triangle 1
			geom->vertices.push_back(vertices[0]);
			geom->texcoords.push_back(glm::vec2(deltaWidth * x, deltaDepth * z));
			geom->vertices.push_back(vertices[3]);
			geom->texcoords.push_back(glm::vec2(deltaWidth * (x + 1), deltaDepth * (z + 1)));
			geom->vertices.push_back(vertices[2]);
			geom->texcoords.push_back(glm::vec2(deltaWidth * x, deltaDepth * (z + 1)));
			// Triangle 2
			geom->vertices.push_back(vertices[0]);
			geom->texcoords.push_back(glm::vec2(deltaWidth * x, deltaDepth * z));
			geom->vertices.push_back(vertices[1]);
			geom->texcoords.push_back(glm::vec2(deltaWidth * (x + 1), deltaDepth * z ));
			geom->vertices.push_back(vertices[3]);
			geom->texcoords.push_back(glm::vec2(deltaWidth * (x + 1), deltaDepth * (z + 1)));

			for (int j = 0; j < 6; ++j)
			geom->normals.push_back(glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)));
		}
	}
	
	initialiseBuffers(geom);

	return geom;
}
//##############################################################################################################
//######################################CreateSierpinski########################################################
void divide_sierpinski(geometry* geom, const glm::vec3 vertices[4], int count)
{	
	if (count > 0)
	{
		glm::vec3 v[6];
		v[0] = vertices[0] + ((vertices[1] - vertices[0]) / 2.0f);
		v[1] = vertices[0] + ((vertices[2] - vertices[0]) / 2.0f);
		v[2] = vertices[0] + ((vertices[3] - vertices[0]) / 2.0f);
		v[3] = vertices[1] + ((vertices[2] - vertices[1]) / 2.0f);
		v[4] = vertices[1] + ((vertices[3] - vertices[1]) / 2.0f);
		v[5] = vertices[2] + ((vertices[3] - vertices[2]) / 2.0f);
		glm::vec3 tempVerts[4];
		// Tetrahedron 1
		tempVerts[0] = vertices[0];
		tempVerts[1] = v[0];
		tempVerts[2] = v[1];
		tempVerts[3] = v[2];
		divide_sierpinski(geom, tempVerts, count - 1);
		// Tetrahedron 2
		tempVerts[0] = v[0];
		tempVerts[1] = vertices[1];
		tempVerts[2] = v[3];
		tempVerts[3] = v[4];
		divide_sierpinski(geom, tempVerts, count - 1);
		// Tetrahedron 3
		tempVerts[0] = v[1];
		tempVerts[1] = vertices[2];
		tempVerts[2] = v[3];
		tempVerts[3] = v[5];
		divide_sierpinski(geom, tempVerts, count - 1);
		// Tetrahedron 4
		tempVerts[0] = v[2];
		tempVerts[1] = vertices[3];
		tempVerts[2] = v[4];
		tempVerts[3] = v[5];
		divide_sierpinski(geom, tempVerts, count - 1);
	}
	else
	{
		for (int i = 0; i < 12; ++i)
		{
			geom->vertices.push_back(vertices[tetrahedron_indices[i]]);
			geom->normals.push_back(vertices[tetrahedron_indices[i]]);
			geom->texcoords.push_back(tetrahedron_texcoords[i % 3]);
		}
	}
};
geometry* createSierpinski(int divisions)
{
	glm::vec3 vertices[4];
	std::memcpy(vertices, tetrahedron_vertices, sizeof(glm::vec3) * 4);
	geometry* geom = new geometry();
	divide_sierpinski(geom, vertices, divisions);

	initialiseBuffers(geom);

	return geom;
}
//##############################################################################################################
//######################################SPHERE BY DIVISION######################################################
void  triangle(geometry* geom, const glm::vec3& a, glm::vec3& b, glm::vec3& c)
{
	glm::vec3 normal = glm::normalize(glm::cross(b-a, c-b));

	geom->vertices.push_back(a);
	geom->vertices.push_back(b);
	geom->vertices.push_back(c);

	geom->normals.push_back(a);
	geom->normals.push_back(b);
	geom->normals.push_back(c);

	/*for (int i = 0; i < 3; ++i)
		geom->normals.push_back(normal);*/

}
void divideTriangle(geometry* geom, const glm::vec3& a, glm::vec3& b, glm::vec3& c, int divisions)
{
	glm::vec3 ortho = (a + b + c)/3.0f;
	glm::vec3 ab = glm::normalize((a + b)/2.0f);
	glm::vec3 ac = glm::normalize((a + c)/2.0f);
	glm::vec3 bc = glm::normalize((b + c)/2.0f);
	glm::vec3 normal = glm::normalize(glm::cross(b-a, c-b));

	if(divisions>0)
	{
		divideTriangle(geom, a, ab, ac, divisions - 1); 
		divideTriangle(geom, ab, b, bc, divisions - 1); 
		divideTriangle(geom, ac, bc, c, divisions - 1); 
		divideTriangle(geom, ab, ac, bc, divisions - 1); 	
	}
	else 
	{
		triangle(geom, a, ab, ac);
		triangle(geom, ab, b, bc);
		triangle(geom, ac, bc, c);
		triangle(geom, ab, ac, bc);
	}
}
geometry* createSphereTri(int divisions)
{
	geometry* geom = new geometry();
	glm::vec3 v[4] = 
	{
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.942809f, -0.333333f),
		glm::vec3(-0.816497f, -0.471405f, -0.333333f),
		glm::vec3(0.816497f, -0.471405f, -0.333333f)
	};
	divideTriangle(geom, v[0], v[1], v[2], divisions);
	divideTriangle(geom, v[3], v[2], v[1], divisions);
	divideTriangle(geom, v[0], v[3], v[1], divisions);
	divideTriangle(geom, v[0], v[2], v[3], divisions);

	initialiseBuffers(geom);

	return geom;
}

void setUpTerrainVertices(geometry* geom, int width, int height)
{
	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
		{
			geom->vertices.push_back(glm::vec3(-((float)width/2.0f) + x, 0.0f, -((float)height / 2.0f) + y));
			geom->texcoords.push_back(glm::vec2((float)x / 10.0f, (float)y / 10.0f));
		}
}

void setUpTerrainIndices(geometry* geom, int width, int height)
{
	int count = 0;
	for (int x = 0; x < width - 1; ++x)
		for (int y = 0; y < height -1; ++y)
		{
			int topLeft = (y * width) + x;
			int topRight = (y * width) + x + 1;
			int bottomLeft = ((y + 1) * width) + x;
			int bottomRight = ((y + 1) * width) + x + 1;

			geom->indices.push_back(topLeft);
			geom->indices.push_back(bottomRight);
			geom->indices.push_back(bottomLeft);

			geom->indices.push_back(topLeft);
			geom->indices.push_back(topRight);
			geom->indices.push_back(bottomRight);
		}
}

void loadHeightData(texture* tex, geometry* geom, int width, int height)
{
	glm::vec4* data = new glm::vec4[width * height];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, data);

	for (int x = 0; x < width; ++x)
		for (int y = 0; y < height; ++y)
		{
			glm::vec3 pos = geom->vertices[(y * width) + x];
			pos.y = data[(y * width) + x].r * 5.0f;
			geom->vertices[(y * width) + x] = pos;
		}

	delete[] data;
}


void calculateTerrainNormals(geometry* geom, int width, int height)
{
	for (int i = 0; i < geom->vertices.size(); ++i)
		geom->normals.push_back(glm::vec3(0.0f, 0.0f, 0.0f));

	for (int i = 0; i < geom->indices.size() / 3; ++i)
	{
		int i1 = geom->indices[i * 3];
		int i2 = geom->indices[i * 3 + 1];
		int i3 = geom->indices[i * 3 + 2];

		glm::vec3 side1 = geom->vertices[i1] - geom->vertices[i3];
		glm::vec3 side2 = geom->vertices[i2] - geom->vertices[i2];
		glm::vec3 normal = glm::cross(side2, side1);

		geom->normals[i1] += normal;
		geom->normals[i2] += normal;
		geom->normals[i3] += normal;
	}

	for (int i = 0; i < geom->normals.size(); ++i)
		geom->normals[i] = glm::normalize(geom->normals[i]);
}

geometry* createTerrain(texture* tex)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, tex->getImageID());
	int width, height;
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
	glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

	geometry* geom = new geometry();

	setUpTerrainVertices(geom, width, height);
	setUpTerrainIndices(geom, width, height);
	loadHeightData(tex, geom, width, height);
	calculateTerrainNormals(geom, width, height);

	initialiseBuffers(geom);

	glGenBuffers(1, &geom->indexBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, geom->indices.size() * sizeof(unsigned int), &geom->indices[0], GL_STATIC_DRAW);

	return geom;
}