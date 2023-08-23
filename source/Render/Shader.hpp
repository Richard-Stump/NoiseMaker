#ifndef SHADER_HPP_INCLUDED
#define SHADER_HPP_INCLUDED

#include <string>

#include "OpenGL.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader
{
public:
	void load(std::string &&name);

	void use() { glUseProgram(_id); }

	void setMat4(std::string&& name, glm::mat4& mat)
	{
		glUniformMatrix4fv(getLoc(name), 1, false, glm::value_ptr(mat));
	}
	void setMat3(std::string&& name, glm::mat3& mat)
	{
		glUniformMatrix3fv(getLoc(name), 1, false, glm::value_ptr(mat));
	}

	void setInt(std::string&& name, int val) { glUniform1i(getLoc(name), val); }
	void setUint(std::string&& name, unsigned int val) { glUniform1ui(getLoc(name), val); }
	void setFloat(std::string&& name, unsigned int val) { glUniform1f(getLoc(name), val); }

	void setVec2(std::string&& name, float x, float y) { glUniform2f(getLoc(name), x, y); }
	void setVec3(std::string&& name, float x, float y, float z) { glUniform3f(getLoc(name), x, y, z); }
	void setVec4(std::string&& name, float x, float y, float z, float w) { glUniform4f(getLoc(name), x, y, z, w); }

	void setVec2(std::string&& name, glm::vec2 v) { glUniform2fv(getLoc(name), 1, glm::value_ptr(v)); }
	void setVec3(std::string&& name, glm::vec3 v) { glUniform3fv(getLoc(name), 1, glm::value_ptr(v)); }
	void setVec4(std::string&& name, glm::vec4 v) { glUniform4fv(getLoc(name), 1, glm::value_ptr(v)); }

	void setBool(std::string&& name, bool v) { glUniform1i(getLoc(name), (int)v); }

protected:
	unsigned int loadShader(std::string &name, unsigned int type, bool required);

	unsigned int getLoc(std::string& name) { return glGetUniformLocation(_id, name.c_str()); }

	unsigned int _id;
};

#endif//SHADER_HPP_INCLUDED