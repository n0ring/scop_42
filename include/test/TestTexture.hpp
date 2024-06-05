#pragma once

#include <memory>
#include "test/Test.hpp"
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" 

namespace test
{
	class TestTexture : public Test
	{
	public:
		TestTexture();
		~TestTexture();

		void onUpdate(float deltaTime) override;
		void onRender() override;
		void onImGuiRender() override;
	private: 
		float m_clearColor[4];

		
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<IndexBuffer> m_IBO;
		std::unique_ptr<Shader>		m_shader;
		std::unique_ptr<Texture>	m_texture;
		std::unique_ptr<VertexBuffer>	m_VBO;

		glm::vec3 m_translationA, m_translationB;
		glm::mat4 m_proj, m_view;
	};
}