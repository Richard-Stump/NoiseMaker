#include "Shader.hpp"

#include <string>
#include <sstream>
#include <iostream>
#include <streambuf>
#include <fstream>

#include "OpenGL.hpp"

using std::cout;
using std::cerr;
using std::endl;

void Shader::load(std::string &&name)
{
	cout << "Loading Shader Program \"" << name << "\":" << endl;

	unsigned int vert = loadShader(name, GL_VERTEX_SHADER, true);
	unsigned int frag = loadShader(name, GL_FRAGMENT_SHADER, true);
	unsigned int geom = loadShader(name, GL_GEOMETRY_SHADER, false);

	_id = glCreateProgram();

	if (vert == 0 || frag == 0)	goto cleanup_return;

	glAttachShader(_id, vert);
	glAttachShader(_id, frag);

	//geometry shaders are optional, so only link it if it exists
	if(geom > 0) 
		glAttachShader(_id, geom);

	glLinkProgram(_id);

	//check the success of linking

	int success;
	glGetProgramiv(_id, GL_LINK_STATUS, &success);

	if (!success)
	{
		int logLength;
		glGetProgramiv(_id, GL_INFO_LOG_LENGTH, &logLength);

		char* log = new char[logLength + 1];

		glGetProgramInfoLog(_id, logLength, nullptr, log);

		cerr << "  Failed to link \"" << name << "\"\n" << log << endl;
	}

cleanup_return:	// frowned upon, but I don't care
	if (vert > 0) glDeleteShader(vert);
	if (frag > 0) glDeleteShader(frag);
	if (geom > 0) glDeleteShader(geom);

	return;
}

unsigned int Shader::loadShader(std::string &name, unsigned int type, bool required)
{
	std::string extension;

	switch (type)
	{
	case GL_VERTEX_SHADER:		extension = ".vert";	break;
	case GL_FRAGMENT_SHADER:	extension = ".frag";	break;
	case GL_GEOMETRY_SHADER:	extension = ".geom";	break;
	default:
		cerr << "  Invalid shader type. Aborting" << endl;
		return 0;
	}

	std::ifstream file(name + extension);

	if (!file.good())
	{
		if (required)
			cout << "  Required shader \"" << name + extension << "\" not found. Cannot continue loading program." << endl;
		else
			cout << "  Shader \"" << name + extension << "\" not found. Skipping." << endl;

		return 0;
	}


	std::string code((std::istreambuf_iterator<char>(file)),
				      std::istreambuf_iterator<char>());

	const char* codeStr = code.c_str();

	unsigned int shader = glCreateShader(type);
	glShaderSource(shader, 1, &codeStr, nullptr);
	glCompileShader(shader);

	//check for the compile error
	int success = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		int logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

		char* log = new char[logLength + 1];

		glGetShaderInfoLog(shader, logLength, nullptr, log);

		cerr << "  Failed to compile \"" << name + extension << "\"\n" << log << endl;

		return 0;
	}
	else
	{
		cout << "  Shader \"" << name + extension << "\" found." << endl;

		return shader;
	}
}