#include "stdafx.h"
#include "cubemap.h"
#include "stb_image.h"

static auto stbi_load_smart = [](auto... args) {
  auto closer_lambda = [](stbi_uc *rgbd) { stbi_image_free(rgbd); };
  std::unique_ptr<stbi_uc, decltype(closer_lambda)> rgb(stbi_load(args...),
                                                        closer_lambda);
  return rgb;
};


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
	// ILuint texid[6];
	unsigned char * data[6];
	int width[6];
	int height[6];
	int bpp[6];
	// Create a DevIL image
	// ilGenImages(6, texid);

	// Try and load each image for the cubemap
	for (int i = 0; i < 6; ++i)
	{
		// Bind Image
		// ilBindImage(texid[i]);

		// Load in the image into memory
		// ILboolean success = ilLoadImage(_filenames[i].c_str());

		// int width, height, bpp;
		//auto load = stbi_load_smart(_filenames[i].c_str(), &width[i], &height[i], &bpp[i], 4);
		//data[i] = load.get();
		//if (data[i] == NULL || width == 0 || height == 0) {
		//	throw std::runtime_error("Error reading texture");
		//}
		// if(!success)
		// 	return success;

		// Conver the image into RGBA
		// success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		// if (!success)
		// {
		// 	// Delete all create images
		// 	// for (int j = 0; j <= i; ++j)
		// 		// ilDeleteImages(1, &texid[j]);
		// 	return success;
		// }
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
		 auto load = stbi_load_smart(_filenames[i].c_str(), &width[i], &height[i], &bpp[i], 4);
		 //data[i] = load.get();
		 //if (data[i] == NULL || width == 0 || height == 0) {
		//	 throw std::runtime_error("Error reading texture");
		 //}

		// ilBindImage(texid[i]);
		glTexImage2D(targets[i],
					 0,
					 bpp[i],
					 width[i],
					 height[i],
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
			load.get());
		CHECK_GL_ERROR
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// Delete the DevIL images
	// ilDeleteImages(6, texid);

	return true;
}