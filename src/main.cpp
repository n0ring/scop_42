#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

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
#include "Texture.hpp"
#include "test/TestClearColor.hpp"
#include "test/TestTexture.hpp"


#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "ParsedObject.hpp"
#include "ObjectRenderer.hpp"

#include "KeyboardManager.hpp"

// ModelState g_modelState;
KeyboardManager keyboardManager;

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	keyboardManager.setKeyState(key, action);
}


int main(void)
{
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessary on Mac

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "scop_42", NULL, NULL);
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


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);

	Renderer renderer; 
	// ObjectRenderer objectRenderer("teapot.obj"); // model states 
	// ObjectRenderer objectRenderer("teapot2.obj"); // model states 
	// ObjectRenderer objectRenderer("sword.obj"); // model states 
	ObjectRenderer objectRenderer("ship.obj"); // model states 
	// ObjectRenderer objectRenderer("42.obj"); // model states 
	// ObjectRenderer objectRenderer("cube.obj"); // model states 
	// ObjectRenderer objectRenderer("new_cube.obj"); // model states 
	// ObjectRenderer objectRenderer("res1/plane.obj"); // model states 
	// ObjectRenderer objectRenderer("res1/alienanimal.obj"); // model states 
	// ObjectRenderer objectRenderer("res1/castle.obj"); // model states 
	// ObjectRenderer objectRenderer("res1/spider.obj"); // model states 
	// ObjectRenderer objectRenderer("res1/building.obj"); // model states 
	// ObjectRenderer objectRenderer("res1/ohouse.obj"); // model states 

	if (objectRenderer.isObjectValid() == false)
	{
		return -1;
	}
	
	glfwSetKeyCallback(window, key_callback);
		glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.clear();


		keyboardManager.update(objectRenderer.getModelState());

		objectRenderer.onRender();
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}