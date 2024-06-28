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

class ObjectRenderer
{
public:
	ObjectRenderer(const ParsedObject &parsedObject);
	~ObjectRenderer() {}
	void onUpdate(float deltaTime);
	void onRender();
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

	float m_rotation_x = 0.0f;
	float m_rotation_y = 0.0f;

};