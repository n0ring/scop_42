#pragma once
#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <unordered_map>
#include "glm/glm.hpp"

struct ShaderProgramSourse
{
	std::string vertexSource;
	std::string fragmentSource;
};

class Shader
{
public:
	Shader(const std::string &filePath);
	~Shader();

	void bind() const;
	void unbind() const;

	void setUniform1i(const std::string &name, int value);
	void setUniform4f(const std::string &name, float v0, float v1, float v2, float v3);
	void setUniformMat4f(const std::string &name, glm::mat4& matrix);
	void setUniformVec3(const std::string &name, glm::vec3 data);



private:
	std::string m_filePath;
	unsigned int m_renderID;
	std::unordered_map<std::string, int> m_uniformLocationCache;

	int getUniformLocation(const std::string &name);
	ShaderProgramSourse parseShader();
	unsigned int compileShader(unsigned int type, const std::string source);
	unsigned int createShader(const std::string &vertexShader, const std::string &fragmentShader);
};