#include "texture.h"
#include "util.h"
#include "stb_image.h"
// #include <IL\il.h>

  static auto stbi_load_smart = [](auto... args) {
  auto closer_lambda = [](stbi_uc *rgbd) { stbi_image_free(rgbd); };
  std::unique_ptr<stbi_uc, decltype(closer_lambda)> rgb(stbi_load(args...),
                                                        closer_lambda);
  return rgb;
};

texture::texture(const std::string& filename)
	: _filename(filename), _image(0)
{
}

texture::~texture()
{
	if (_image) glDeleteTextures(1, &_image);
	_image = 0;
}

bool texture::create()
{
	// ILuint texid;
	// ilGenImages(1, &texid);
	// ilBindImage(texid);
	// ILboolean success = ilLoadImage(_filename.c_str());
	int width, height, bpp;
  	auto rgb = stbi_load_smart(_filename.c_str(), &width, &height, &bpp, 4);
	if (rgb != NULL && width != 0 && height != 0)
	{
		// success = ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		// if (!success)
		// {
		// 	// ilDeleteImages(1, &texid);
		// 	return success;
		// }
		glGenTextures(1, &_image);
		glBindTexture(GL_TEXTURE_2D, _image);

		// Set magnification and minification filters for the image
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		CHECK_GL_ERROR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		CHECK_GL_ERROR

		// Set texture repeat
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		CHECK_GL_ERROR
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		CHECK_GL_ERROR

		// set the anisotropy	
		float maxAnisotropy;
		
		glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
		CHECK_GL_ERROR
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
		CHECK_GL_ERROR
			glTexImage2D(GL_TEXTURE_2D,
						 0,
						 bpp,
						 width,
						 height,
						 0,
						 GL_RGBA,
						 GL_UNSIGNED_BYTE,
						 rgb.get());
			CHECK_GL_ERROR
			glGenerateMipmap(GL_TEXTURE_2D);
			CHECK_GL_ERROR
			// ilDeleteImages(1, &texid);
	}
	else {
		return false;
		//std::cout << "Error reading texture\n";
		//throw std::runtime_error("Error reading texture");
	}
	return true;
}

bool texture::create(int width, int height, glm::vec4* data)
{
	// Generate the texture buffer
	glGenTextures(1, &_image);
	CHECK_GL_ERROR

	glBindTexture(GL_TEXTURE_2D, _image);
	CHECK_GL_ERROR

	// Set magnification and minification filters for the image
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	CHECK_GL_ERROR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	CHECK_GL_ERROR

	//Set Repeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	CHECK_GL_ERROR
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	CHECK_GL_ERROR

	float maxAnisotropy;
	glGetFloatv(GL_MAX_TEXTURE_MAX_ANISOTROPY_EXT, &maxAnisotropy);
	CHECK_GL_ERROR
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAX_ANISOTROPY_EXT, maxAnisotropy);
	CHECK_GL_ERROR

	// Create the image
	glTexImage2D(GL_TEXTURE_2D,
				 0,
				 GL_RGBA,
				 width,
				 height,
				 0,
				 GL_RGBA,
				 GL_FLOAT,
				 data);
	CHECK_GL_ERROR

	//Generate the mipmaps
	glGenerateMipmap(GL_TEXTURE_2D);
	CHECK_GL_ERROR

	return true;
}