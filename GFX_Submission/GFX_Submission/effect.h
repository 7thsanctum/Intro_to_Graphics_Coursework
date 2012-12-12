#pragma once

#include <string>
#include <hash_map>
#include <GL\glew.h>
#include <glm\glm.hpp>

class effect
{
private:
	GLuint _program;
	std::vector<GLuint> _shaders;
	std::hash_map<std::string, GLint> _uniforms;
	std::hash_map<std::string, GLint> _blockUniforms;
public:
	effect();
	~effect();
	void begin();
	void end();
	bool addShader(const std::string& filename, GLenum type);
	bool create();
	GLint getUniformIndex(const std::string& name) const { return _uniforms.at(name); }
	GLint getUniformBlockIndex(const std::string& name) const { return _blockUniforms.at(name); }
};

GLuint loadShader(const std::string& filename, GLenum type);

GLuint createProgram(GLuint* shaders, int count);