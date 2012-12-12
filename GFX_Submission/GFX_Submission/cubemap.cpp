#include "cubemap.h"


cubemap::cubemap(const std::vector<std::string>& filenames)
	: _filenames(filenames), _image(0)
{
}

cubemap::~cubemap()
{
	if(_image) glDeleteTextures(1, &_image);
	_image = 0;
}

bool cubemap::create()
{
	// First use DevIL to load in the image data
	ILuint texid[6];

	// Create a DevIL image
	ilGenImages(6, texid);

	// Try and load each image for the cubemap
	for (int i = 0; i <6; ++i)
	{
		// Bind Image
		ilBindImage(texid[i]);

		// Load in the image into memory
		ILboolean success = ilLoadImage((wchar_t*)_filenames[i].c_str());
		if(!success)
			return success;

		// Conver the image into RGBA
		success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		if (!success)
		{
			// Delete all create images
			for (int j = 0; j <= i; ++j)
				ilDeleteImages(1, &texid[j]);
			return success;
		}
	}

	glActiveTexture(GL_TEXTURE0);

	// Enable cube map state
	glEnable(GL_TEXTURE_CUBE_MAP);
	CHECK_GL_ERROR

	// Now use OpenGL to create the image buffer
	glGenTextures(1, &_image);
	CHECK_GL_ERROR

	// Bind texture as cubemap
	glBindTexture(GL_TEXTURE_CUBE_MAP, _image);
	CHECK_GL_ERROR

	// Set Up texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	CHECK_GL_ERROR

	// Create the cube map image by assigning each texture
	GLenum targets[6] = 
	{
		GL_TEXTURE_CUBE_MAP_POSITIVE_X,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
		GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
		GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
	};

	for(int i = 0; i < 6; ++i)
	{
		ilBindImage(texid[i]);
		glTexImage2D(targets[i],
					 0,
					 ilGetInteger(IL_IMAGE_BPP),
					 ilGetInteger(IL_IMAGE_WIDTH),
					 ilGetInteger(IL_IMAGE_HEIGHT),
					 0,
					 ilGetInteger(IL_IMAGE_FORMAT),
					 GL_UNSIGNED_BYTE,
					 ilGetData());
		CHECK_GL_ERROR
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Delete the DevIL images
	ilDeleteImages(6, texid);

	return true;
}