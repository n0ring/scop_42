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
#include "ParsedObject.hpp"

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

	ObjectRenderer lightRenderer("elder.obj", "res/shaders/lightObject.shader", "res/texture/elder_tex.jpg");

	// ObjectRenderer objectRenderer("teapot2.obj", "res/shaders/Basic.shader", "res/texture/tex1.bmp"); // model states 
	// ObjectRenderer objectRenderer("teapot.obj"); // model states 
	// ObjectRenderer objectRenderer("sword.obj"); // model states 
	// ObjectRenderer objectRenderer("ship.obj", "res/shaders/Basic.shader", "res/texture/tex1.bmp");
	// ObjectRenderer objectRenderer("42.obj", "res/shaders/Basic.shader", "res/texture/elder_tex.jpg"); // model states 
	// ObjectRenderer objectRenderer("cube.obj"); // model states 
	ObjectRenderer objectRenderer("new_cube.obj", "res/shaders/Basic.shader", "res/texture/tex1.bmp"); // model states 
	// ObjectRenderer objectRenderer("res1/plane.obj"); // model states 
	// ObjectRenderer objectRenderer("res1/alienanimal.obj"); // model states 
	// ObjectRenderer objectRenderer("res1/castle.obj", "res/shaders/Basic.shader", "res/texture/tex1.bmp"); // model states 
	// ObjectRenderer objectRenderer("res1/spider.obj"); // model states 
	// ObjectRenderer objectRenderer("res1/building.obj"); // model states 
	// ObjectRenderer objectRenderer("res1/ohouse.obj", "res/shaders/Basic.shader"); // model states 
	// ObjectRenderer objectRenderer("elder1.obj", "res/shaders/Basic.shader");

	if (objectRenderer.isObjectValid() == false)
	{
		return -1;
	}
	
	glfwSetKeyCallback(window, key_callback);
		glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	
	keyboardManager.addObserver(&objectRenderer);
	keyboardManager.addObserver(&lightRenderer);
	lightRenderer.getModelState().setElder();
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		renderer.clear();


		keyboardManager.update();
		keyboardManager.spellCheck();

		objectRenderer.onRender();
		if (lightRenderer.getModelState().lightOn)
			lightRenderer.onRender();
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
		objectRenderer.getModelState().lightPos = lightRenderer.getObjectCenter();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}