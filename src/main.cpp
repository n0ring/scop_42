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
#include "Shader.hpp"
#include "Renderer.hpp"


int main(void)
{
	GLFWwindow *window;

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
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}
	std::cout << glGetString(GL_VERSION) << std::endl;

	float positions[] = {
		-0.5f, -0.5f,
		0.5f, -0.5f,
		0.5f, 0.5f,
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

	Shader shader("res/shaders/Basic.shader");
	shader.bind();
	float red = 0.05f;

	va.unbind();
	vb.unbind();
	ib.unbind();
	shader.unbind();

	Renderer renderer;


	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_LINE

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		/* Render here */
		renderer.clear();
		shader.bind();
		shader.setUniform4f("our_color", red, 0.0f, 0.8f, 1.0f);

		renderer.draw(va, ib, shader);

		if (red >= 1.0f)
			red = 0.05f;
		red += 0.007f;

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// glDeleteProgram(shader);
	// glDeleteBuffers(1, &vbo);
	glfwTerminate();
	// glDeleteVertexArrays(1, &vao);
	return 0;
}