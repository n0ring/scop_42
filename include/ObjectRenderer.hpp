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
#define SCALE_STEP 0.01f

struct ModelState
{
	float rotation_x = 0.0f;
	float rotation_y = 0.0f;
	glm::vec3 scale;
	ModelState() : scale(1.0f, 1.0f, 1.0f) {}




	void moveX(float val)
	{
		rotation_x += val;
		if (rotation_x >= 360)
			rotation_x = 0;
		else if (rotation_x <= 0)
			rotation_x = 360;
	}

	void moveY(float val)
	{
		rotation_y += val;
		if (rotation_y >= 360)
			rotation_y = 0;
		else if (rotation_y <= 0)
			rotation_y = 360;
	}

	void moveUp()
	{
		moveY(-MOVE_STEP);
	}
	void moveDown()
	{
		moveY(MOVE_STEP);
	}
	void moveLeft()
	{
		moveX(-MOVE_STEP);
	}
	void moveRight()
	{
		moveX(MOVE_STEP);
	}

	void increaseScale()
	{
		scale.z += SCALE_STEP;
		scale.x += SCALE_STEP;
		scale.y += SCALE_STEP;
		if (scale.x > 2.5f)
		{
			scale.z = 2.5f;
			scale.x = 2.5f;
			scale.y = 2.5f;
		}
	}

	void decreaseScale()
	{
		scale.z -= SCALE_STEP;
		scale.x -= SCALE_STEP;
		scale.y -= SCALE_STEP;
		if (scale.z < 0)
		{
			scale.z = 0;
			scale.x = 0;
			scale.y = 0;
		}
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