#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <math.h>
#include <vector> 
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"


// #define ASSERT(x) if (!(x)) __debugbreak();
// #define GLCall(x) clearError();\
// 	x;\
// 	ASSERT(checkError(#x, __FILE__, __LINE__))



// static void clearError()
// {
// 	while (glGetError() != GL_NO_ERROR);
// }

// static void checkError()
// {
// 	while (GLenum error = glGetError())
// 	{
// 		std::cout << "[OpenGL Error] (" << error << ")" << std::endl;
// 	}
// }



struct ShaderProgramSourse
{
	std::string vertexSource;
	std::string fragmentSource;
};


static ShaderProgramSourse parseShader(const std::string& filepath)
{
	std::ifstream file(filepath);
	std::string line;
	
	enum class ShaderType
	{
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	ShaderType type = ShaderType::NONE;
	std::stringstream ss[2];


	while (getline(file, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
				type = ShaderType::VERTEX;
			else if (line.find("fragment") != std::string::npos)
				type = ShaderType::FRAGMENT;
		}
		else 
		{
			ss[(int)type] << line << '\n';
		}
	}

	return { ss[0].str(), ss[1].str() };
}

static unsigned int compileShader(unsigned int type, const std::string source)
{   
	unsigned int id = glCreateShader(type);
	const char* src = source.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result); 
	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);
		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
		std::cout << message << std::endl;
		glDeleteShader(id);
		return 0;
	}

	return id;
}

static unsigned int createShader(const std::string& vertexShader, const std::string& fragmentShader)
{
	int linkResult;
	unsigned int program = glCreateProgram();
	unsigned int vs = compileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = compileShader(GL_FRAGMENT_SHADER , fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	glGetProgramiv(program, GL_LINK_STATUS, &linkResult);
	if (linkResult == GL_FALSE)
	{
		int length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char *message = (char*)alloca(length * sizeof(char));
		glGetProgramInfoLog(program, length, &length, message);
		std::cout << "Failed to link the shader program!" << std::endl;
		std::cout << message << std::endl;
		glDeleteProgram(program);
		return 0;
	}


	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


int main(void)
{
	GLFWwindow* window;
	

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	// glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessary on Mac


	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f,  0.5f,
		-0.5f, 0.5f,
	};

	unsigned int indecies[] = {
		0, 1, 2, 
		3, 2, 0
	};

	VertexArray va;
	VertexBuffer vb(positions, 8 * sizeof(float));

	VertexBufferLayout layouts;
	layouts.push<float>(2);
	va.addBuffer(vb, layouts);

// glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0); // 0 is the index of the vertex attribute
	IndexBuffer ib(indecies, 6);
	ShaderProgramSourse source;

	source = parseShader("res/shaders/Basic.shader");
	unsigned int shader = createShader(source.vertexSource, source.fragmentSource);

	glUseProgram(shader);
	int colors = glGetUniformLocation(shader, "our_color");
	float red = 0.05f;

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
 
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_LINE

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);


		glUseProgram(shader);
		glUniform4f(colors, red, 0.0f, 0.8f, 1.0f);

		va.bind();
		ib.bind();
		

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

		if (red >= 1.0f)
			red = 0.05f;
		red += 0.007f;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	// glDeleteBuffers(1, &vbo);
	glfwTerminate();
	// glDeleteVertexArrays(1, &vao);
	return 0;
}