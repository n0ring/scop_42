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

int main(int argc, char** argv)
{
	GLFWwindow *window;
	std::vector<ObjectRenderer> objects;
	unsigned int objId = 0;

	if (argc == 1)
	{
		std::cout << "Usage of scop: \n";
		std::cout << "For render model add path to .obj file as second argument. Example: ./scop pathTo.obj\n";
		std::cout << "Controls: \n";
		std::cout << "WASD for move object in xy axis\n";
		std::cout << "Q E  for move object in z axis\n";
		std::cout << "UP DOWN LEFT RIGHT  for turn object\n";
		std::cout << "T for toggle between texture color modes\n";
		std::cout << "F for lines only. U for fill mode\n";
		std::cout << "- + for scale object (use left shit for +)\n";
		std::cout << "For turn on light type lumos. Guess for turn off?\n";
		return 0;

	}
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


	objects.push_back(ObjectRenderer("res/models/elder.obj", objId++, "res/shaders/lightObject.shader", "res/texture/elder_tex.jpg"));
	for (int i = 1; i < argc; i++)
	{
		objects.push_back(ObjectRenderer(argv[i], objId++));
	}

	for (auto& obj : objects)
	{
		if (obj.isObjectValid() == false)
		{
			return -1;
		}
		keyboardManager.addObserver(&obj);
	}
	
	glfwSetKeyCallback(window, key_callback);
		glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	
	objects[0].getModelState().setElder();
	objects[1].setObjectAsActive();
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	while (!glfwWindowShouldClose(window))
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		keyboardManager.update();
		keyboardManager.spellCheck();
		for (int i = 1; i < objects.size(); i++)
		{
			objects[i].onRender();
			objects[i].getModelState().lightPos = objects[0].getObjectCenter();
		}
		if (objects[0].getModelState().lightOn)
			objects[0].onRender();
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}

 // add is active