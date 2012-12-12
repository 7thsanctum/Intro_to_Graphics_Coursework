#pragma once

#include <string>
#include <GL\glew.h>
#include <IL\il.h>
#include "util.h"
#include <vector>

class cubemap
{
private:
	GLuint _image;

	std::vector<std::string> _filenames;
public:
	cubemap(const std::vector<std::string>& filenames);

	~cubemap();

	GLuint getImageID() const { return _image; }

	bool create();
};












































