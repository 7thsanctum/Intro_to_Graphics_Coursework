#include "effect.h"
#include <iostream>
#include <fstream>
#include <sstream>

effect::effect()
{
}

effect::~effect()
{
	if (_program) glDeleteProgram(_program);
	_program = 0;
	for (int i = 0; i < _shaders.size(); ++i)
		glDeleteShader(_shaders[i]);
	_shaders.clear();
}

void effect::begin()
{
	glUseProgram(_program);
}

void effect::end()
{
	glUseProgram(0);
}

bool effect::addShader(const std::string& filename, GLenum type)
{
	GLuint shader = loadShader(filename, type);
	if (!shader)
		return false;
	_shaders.push_back(shader);
	return true;
}

bool effect::create()
{
	_program = createProgram(&_shaders[0], _shaders.size());
	if (!_program)
		return false;

	std::vector<std::string> uniformNames;
	GLint numUniforms, maxLength;
	glGetProgramiv(_program, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxLength);
	glGetProgramiv(_program, GL_ACTIVE_UNIFORMS, &numUniforms);
	char* buf = new char[maxLength];
	for (int i = 0; i < numUniforms; ++i)
	{
		GLsizei size;
		glGetActiveUniformName(_program, i, maxLength, &size, buf);
		std::string name(buf);
		if (uniformNames.size() > 0 && uniformNames[i - 1] == name)
			break;
		else
			uniformNames.push_back(name);
	}
	delete[] buf;

	for (int i = 0; i < uniformNames.size(); ++i)
	{
		GLint uniformLocation = glGetUniformLocation(_program, uniformNames[i].c_str());
		if (uniformLocation != -1) _uniforms[uniformNames[i]] = uniformLocation;
	}

	std::vector<std::string> uniformBlockNames;
	glGetProgramiv(_program, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxLength);
	glGetProgramiv(_program, GL_ACTIVE_UNIFORM_BLOCKS, &numUniforms);
	buf = new char[maxLength];
	for (int i = 0; i < numUniforms; ++i)
	{
		GLsizei size;
		glGetActiveUniformBlockName(_program, i, maxLength, &size, buf);
		std::string name(buf);
		if (uniformBlockNames.size() > 0 && uniformBlockNames[i - 1] == name)
			break;
		else
			uniformBlockNames.push_back(name);
	}
	delete[] buf;

	for (int i = 0; i < uniformBlockNames.size(); ++i)
	{
		GLuint blockIndex = glGetUniformBlockIndex(_program, uniformBlockNames[i].c_str());
		glUniformBlockBinding(_program, blockIndex, i);
		_blockUniforms[uniformBlockNames[i]] = i;
	}

	return true;
}

std::string readFile(const std::string& filename)
{
	std::ifstream file(filename);
	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string content = buffer.str();
	return content;
}

GLuint loadShader(const std::string& filename, GLenum type)
{
	std::string fileContent = readFile(filename);
	GLuint shader = glCreateShader(type);
	const char* source = fileContent.c_str();
	glShaderSource(shader, 1, &source, 0);
	glCompileShader(shader);

	GLint compiled;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
	if (!compiled)
	{
		GLsizei length;
		char log[1024];
		glGetShaderInfoLog(shader, 1024, &length, log);
		std::cout << "Could not compile shader: " << shader << ": " << filename << std::endl;
		std::cout << log << std::endl;
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint createProgram(GLuint* shaders, int count)
{
	GLuint program = glCreateProgram();
	for (int i = 0; i < count; ++i)
		glAttachShader(program, shaders[i]);
	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);
	if (!linked)
	{
		GLsizei length;
		char log[1024];
		glGetProgramInfoLog(program, 1024, &length, log);
		std::cout << "Error linking program." << std::endl;
		std::cout << log << std::endl;
		for (int i = 0; i < count; ++i)
			glDetachShader(program, shaders[i]);
		glDeleteProgram(program);
		return 0;
	}
	return program;
}