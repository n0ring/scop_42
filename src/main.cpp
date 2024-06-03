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

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

 
void split(std::string& line, std::vector<std::string>& words)
{
	std::string word;
	std::stringstream ss(line);
	words.clear();
	while (ss >> word)
	{
		words.push_back(word);
	}
}

bool parseFile(const std::string& fileName, std::vector<float>& pos, std::vector<unsigned int>& idx)
{
	(void) pos;
	(void) idx;

	std::string line;
	std::ifstream file(fileName);
	std::vector<std::string> words;

	if (file.is_open() == false)
		return false;
	
	while (getline(file, line))
	{
		if (line[0] == 'v' || line[0] == 'f')
		{
			split(line, words);
			if (words[0] == "v")
			{
				pos.push_back(std::stof(words[1]));
				pos.push_back(std::stof(words[2]));
				pos.push_back(std::stof(words[3]));
			}
			if (words[0] == "f")
			{
				idx.push_back(std::stoi(words[1]));
				idx.push_back(std::stoi(words[2]));
				idx.push_back(std::stoi(words[3]));
			}
		}
	}
	std::cout << pos.size() << " " << idx.size() << std::endl;
	return true;
}

int main(void)
{
	// std::vector<float> pos;
	// std::vector<unsigned int> idx;
	// parseFile("test.obj", pos, idx);
	/////
	GLFWwindow *window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // Necessary on Mac

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(960, 540, "Hello World", NULL, NULL);
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


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    const char* glsl_version = "#version 150";
    ImGui_ImplOpenGL3_Init(glsl_version);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_LINE

	test::TestClearColor test;
	while (!glfwWindowShouldClose(window))
	{
		renderer.clear();

		test.onRender();
		test.onUpdate(0.0f);


		// Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

		test.onImGuiRender();
 

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

	glfwTerminate();
	// glDeleteVertexArrays(1, &vao);
	return 0;
}