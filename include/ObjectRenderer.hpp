#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include <memory>
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "ParsedObject.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#define MOVE_STEP 1.0f

struct ModelState
{
	float rotation_x = 0.0f;
	float rotation_y = 0.0f;

	void moveUp()
	{
		rotation_y -= MOVE_STEP;
		if (rotation_y >= 360)
			rotation_y = 0;
	}
	void moveDown()
	{
		rotation_y += MOVE_STEP;
		if (rotation_y >= 360)
			rotation_y = 0;
	}
	void moveLeft()
	{
		rotation_x -= MOVE_STEP;
		if (rotation_y >= 360)
			rotation_y = 0;
	}
	void moveRight()
	{
		rotation_x += MOVE_STEP;
		if (rotation_y >= 360)
			rotation_y = 0;
	}
};

class ObjectRenderer
{
public:
	ObjectRenderer(const ParsedObject &parsedObject);
	~ObjectRenderer() {}
	void onUpdate(float deltaTime);
	void onRender(ModelState&);
	void onImGuiRender(int& fill);


private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IBO;
	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<VertexBuffer> m_VBO;

	glm::vec3 m_translation;
	glm::mat4 m_proj, m_view;

	glm::vec3 view_vec;
};