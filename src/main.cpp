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

	std::vector<float> positions = {
		-50.0f, -50.10f, 0.0f, 0.0f, // 0
		50.0f, -50.0f, 1.0f, 0.0f, // 1
		50.0f, 50.0f, 1.0f, 1.0f, // 2
		-50.0f, 50.0f, 0.0f, 1.0f, // 3

		// 0.0f, 0.7f,
		// 0.0f, 0.5f,
		// -0.5f, -0.5f
	};

	std::vector<unsigned int> indecies = {
		0, 1, 2,
		0, 2, 3
	};


	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
	glEnable(GL_BLEND);

	VertexArray va;
	VertexBuffer vb(positions.data(), positions.size() * sizeof(float));
	VertexBufferLayout layouts;

	layouts.push<float>(2);
	layouts.push<float>(2);
	va.addBuffer(vb, layouts);
	IndexBuffer ib(indecies.data(), indecies.size());

	glm::mat4 proj = glm::ortho(0.0f, 960.0f, 0.0f, 540.0f, -1.0f, 1.0f);
	glm::mat4 view = glm::translate(glm::mat4(1.0f), glm::vec3(-0, 2, 1));


	Shader shader("res/shaders/Basic.shader");
	shader.bind();
	float red = 0.05f;
	shader.setUniform1i("u_Texture", 0);

	Texture texture("res/texture/texture.png");
	texture.bind(0);

	va.unbind();
	vb.unbind();
	ib.unbind();
	shader.unbind();

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

	#ifdef __EMSCRIPTEN__
    ImGui_ImplGlfw_InstallEmscriptenCanvasResizeCallback("#canvas");
	#endif
    ImGui_ImplOpenGL3_Init(glsl_version);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_LINE
	float f = 0.0f;
	glm::vec3 translationA(200, 200, 0);
	glm::vec3 translationB(400, 400, 0);


	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{

		/* Render here */
		renderer.clear();

		// Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();


		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationA) ;
			glm::mat4 mvp = proj * view * model;
			shader.setUnoformMat4f("u_MVP", mvp);
			shader.bind();

			renderer.draw(va, ib, shader);
		}
		{
			glm::mat4 model = glm::translate(glm::mat4(1.0f), translationB) ;
			glm::mat4 mvp = proj * view * model;
			shader.setUnoformMat4f("u_MVP", mvp);
			shader.bind();
			
			renderer.draw(va, ib, shader);
		}

			shader.bind();

        // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
        {
            ImGui::SliderFloat3("translationA", &translationA.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f 
            ImGui::SliderFloat3("translationB", &translationB.x, 0.0f, 960.0f);            // Edit 1 float using a slider from 0.0f to 1.0f 
            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
            // ImGui::End();
		}


		shader.setUniform4f("our_color", red, 0.0f, 0.8f, 1.0f);


		if (red >= 1.0f)
			red = 0.05f;
		red += 0.007f;

		/* Swap front and back buffers */
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