#pragma once

#include <glm\glm.hpp>
#include "effect.h"

#define MAX_LIGHTS 10

struct lighting_data
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 lightDir;
};

struct lighting
{
	lighting_data data;
	GLuint buffer;

	~lighting()
	{
		if (buffer) glDeleteBuffers(1, &buffer);
	}

	void create()
	{
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(lighting_data), &data, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void bind(const effect* eff)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, eff->getUniformBlockIndex("Lighting"), buffer, 0, sizeof(lighting_data));
	}
};

struct point_light_data
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 lightPos;
	glm::vec4 attenuation;
	float dist;
	float _pad[3];
};

struct spot_light_data
{
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	glm::vec4 lightPos;
	glm::vec4 attenuation;
	glm::vec4 lightDir;
	float power;
	float dist;
	float _pad[2];
};

struct dynamic_lighting_data
{
	point_light_data points[MAX_LIGHTS];
	spot_light_data spots[MAX_LIGHTS];
};

struct dynamic_lights
{
	dynamic_lighting_data data;
	GLuint buffer;

	int numPoints;
	int numSpots;

	dynamic_lights() : numPoints(0), numSpots(0), buffer(0)
	{
	}

	~dynamic_lights()
	{
		if (buffer) glDeleteBuffers(1, &buffer);
	}

	void addPoint(const point_light_data& pointLight)
	{ 
		data.points[numPoints] = pointLight;
		++numPoints;
	}

	void addSpot(const spot_light_data& spotLight)
	{
		data.spots[numSpots] = spotLight;
		++numSpots;
	}

	void setPoint(int idx, const point_light_data& pointLight)
	{
		data.points[idx] =  pointLight;
		if (buffer)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, buffer);
			glBufferSubData(GL_UNIFORM_BUFFER,
							idx * sizeof(point_light_data),
							sizeof(point_light_data),
							&data.points[idx]);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	}

	void setSpot(int idx, const spot_light_data& spotLight)
	{
		data.spots[idx] = spotLight;
		if (buffer)
		{
			glBindBuffer(GL_UNIFORM_BUFFER, buffer);
			glBufferSubData(GL_UNIFORM_BUFFER,
							MAX_LIGHTS * sizeof(point_light_data) + idx * sizeof(spot_light_data),
							sizeof(spot_light_data),
							&data.spots[idx]);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);
		}
	}

	void create()
	{
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(dynamic_lighting_data), &data, GL_DYNAMIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void bind(const effect* eff)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER,
						  eff->getUniformBlockIndex("DynamicLights"),
						  buffer,
						  0,
						  sizeof(dynamic_lighting_data));
		glUniform1i(eff->getUniformIndex("numSpots"), numSpots);
		glUniform1i(eff->getUniformIndex("numPoints"), numPoints);
	}
};