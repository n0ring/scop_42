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

	// ParsedObject parsedObject("teapot2.obj", g_modelState);
	// ParsedObject parsedObject("cube.obj", g_modelState);
	// ParsedObject parsedObject("new_cube.obj", g_modelState);
	// ParsedObject parsedObject("teapot.obj", g_modelState);
	// ParsedObject parsedObject("42.obj", g_modelState);

	// if (parsedObject.getParseStatus() == false)
	// 	return -1;
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
	// ObjectRenderer objectRenderer("teapot2.obj"); // model states 
	ObjectRenderer objectRenderer("ship.obj"); // model states 

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); 
	(void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 150";
    ImGui_ImplOpenGL3_Init(glsl_version);
	
	glfwSetKeyCallback(window, key_callback);
		glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.5f, 0.5f, 0.7f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		// glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		renderer.clear();


		// Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
		keyboardManager.update(objectRenderer.getModelState());

		objectRenderer.onRender();
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// glDeleteProgram(shader);
	// glDeleteBuffers(1, &vbo);

	ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
	// delete currentTest;
	// if (currentTest != testMenu)
	// 	delete testMenu;

	glfwTerminate();
	// glDeleteVertexArrays(1, &vao);
	return 0;
}