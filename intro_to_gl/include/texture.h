#pragma once

#include <string>
#include <GL\glew.h>
#include <glm\glm.hpp>

class texture
{
private:
	GLuint _image;
	std::string _filename;
public:
	texture(const std::string& filename);
	~texture();
	GLuint getImageID() const { return _image; }
	bool create();
	bool create(int width, int height, glm::vec4* data);
};