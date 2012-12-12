#pragma once

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\gtx\quaternion.hpp>

struct transform
{
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

	transform()
	{
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
	}

	void move(const glm::vec3& movement)
	{
		position += movement;
	}

	void rotate(float angle, const glm::vec3& axis)
	{
		rotation = glm::rotate(rotation, glm::degrees(angle), axis);
	}

	glm::mat4 getTransformMatrix() const
	{
		glm::mat4 matrix = glm::translate(glm::mat4(1.0f), position);
		matrix = glm::scale(matrix, scale);
		glm::mat4 rotMatrix = glm::mat4_cast(rotation);
		matrix *= rotMatrix;
		return matrix;
	}
};