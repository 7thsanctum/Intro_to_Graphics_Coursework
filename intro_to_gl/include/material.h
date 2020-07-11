#pragma once

#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\glm.hpp>
#include "effect.h"
#include "texture.h"

struct material_data
{
	glm::vec4 emissive;
	glm::vec4 ambient;
	glm::vec4 diffuse;
	glm::vec4 specular;
	float shininess;
	float padding[3];
};

struct material
{
	material_data data;
	GLuint buffer;
	texture *texture;

	material() : texture(0)
	{
	}

	~material()
	{
		if (buffer)
			glDeleteBuffers(1, &buffer);
	}

	void create()
	{
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_UNIFORM_BUFFER, buffer);
		glBufferData(GL_UNIFORM_BUFFER, sizeof(material_data), &data, GL_STATIC_DRAW);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}

	void bind(const effect *eff)
	{
		glBindBufferRange(GL_UNIFORM_BUFFER, eff->getUniformBlockIndex("Material"), buffer, 0, sizeof(material_data));
		if (texture)
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, texture->getImageID());
			glUniform1i(eff->getUniformIndex("tex"), 0);
		}
	}
};