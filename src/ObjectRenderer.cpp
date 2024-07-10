#include "ObjectRenderer.hpp"

ObjectRenderer::ObjectRenderer(const std::string& objectFileName) 
	: m_translation(0, 0, 0),
	//   m_proj(glm::ortho(-5.0f, 5.0f, -3.0f, 3.0f, -5.0f, 5.0f)),
	m_proj(glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.1f, 20.0f)),
	m_view(glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0))),
	view_vec(0, 0, -7),
	m_parsedObject(objectFileName, m_modelState)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	m_VAO = std::make_unique<VertexArray>();


	m_VBO = std::make_unique<VertexBuffer>(m_parsedObject.getPositions().data(), m_parsedObject.getPositions().size() * sizeof(float));
	VertexBufferLayout layouts;
	layouts.push<float>(3); // size of data 
	layouts.push<float>(2); // tex coords
	m_VAO->addBuffer(*m_VBO, layouts, m_parsedObject.getPositions().size());
	m_IBO = std::make_unique<IndexBuffer>(m_parsedObject.getIndices().data(), m_parsedObject.getIndices().size());

	m_shader = std::make_unique<Shader>("res/shaders/Basic.shader");

	m_shader->bind();

	m_texture = std::make_unique<Texture>("res/texture/tex1.bmp");
	m_texture->bind();
	m_shader->setUniform1i("u_Texture", 0);
	m_shader->setUniform1i("u_RenderMode", static_cast<int>(RenderMode::COLOR));
}

void ObjectRenderer::onRender()
{
	glm::mat4 model(1.0f);

	Renderer renderer;
	m_texture->bind();
	model = glm::translate(model, m_modelState.centerOffset);
	model = glm::translate(model, m_modelState.translation); // move model. rotate have to be after move 

	model = glm::rotate(model, glm::radians(m_modelState.rotation_x), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(m_modelState.rotation_y), glm::vec3(1.0f, 0.0f, 0.0f));

	model = glm::translate(model, -m_modelState.centerOffset);

	model = glm::scale(model, m_modelState.scale);

	m_view = glm::translate(glm::mat4(1.0f), view_vec);
	glm::mat4 mvp = m_proj * m_view * model;
	m_shader->bind();
	m_shader->setUniformMat4f("u_MVP", mvp);
	m_shader->setUniform1i("u_RenderMode", static_cast<int>(m_modelState.renderMode));

	if (!m_modelState.fill_model)
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); // GL_LINE
	else 
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // GL_LINE
	clearError();
	renderer.draw(*m_VAO, *m_IBO, *m_shader);
	checkError();
}

void ObjectRenderer::onImGuiRender(int& fill)
{
}