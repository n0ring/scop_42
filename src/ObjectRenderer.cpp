
#include "ObjectRenderer.hpp"

ObjectRenderer::ObjectRenderer(const std::string& objectFileName, const std::string& shaderFileName, const std::string& textureFileName) 
	: m_translation(0, 0, 0),

	// m_proj(glm::perspective(glm::radians(45.0f), 960.0f / 540.0f, 0.01f, 200.0f)),
	m_proj(nrg::vec4(1.358f, 0.0f, 0.0f, 0.0f),
					nrg::vec4(0.0f, 2.41421, 0.0f, 0.0f), 
					 nrg::vec4(0.0f, 0.0f, -1.0001f, -1.0f), 
					 nrg::vec4(0.0f, 0.0f, -0.020001, 0.0f)),
	m_view(nrg::translate(nrg::mat4(1.0f), nrg::vec3(0, 0, 0))),
	view_vec(0, 0, -7)
{	
	m_parsedObject = std::make_unique<ParsedObject>(objectFileName, m_modelState);
	if (m_parsedObject->getParseStatus() == false)
		return ;
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	m_VAO = std::make_unique<VertexArray>();
	if (objectFileName != "elder.obj")
		m_UBO = std::make_unique<UniformBuffer>(m_parsedObject->getMaterials().data(), m_parsedObject->getMaterials().size());

	m_VBO = std::make_unique<VertexBuffer>(m_parsedObject->getPositions().data(), m_parsedObject->getPositions().size() * sizeof(vertex));
	VertexBufferLayout layouts;
	layouts.push<float>(3); // size of data 
	layouts.push<float>(2); // tex coords
	layouts.push<float>(3); // normals coords
	layouts.push<float>(1); // mtl idx
	m_VAO->addBuffer(*m_VBO, layouts, m_parsedObject->getPositions().size() * 9);
	m_IBO = std::make_unique<IndexBuffer>(m_parsedObject->getIndices().data(), m_parsedObject->getIndices().size());

	m_shader = std::make_unique<Shader>(shaderFileName, m_parsedObject->getVaribles());
	m_shader->bind();

	m_texture = std::make_unique<Texture>(textureFileName);
	m_texture->bind();
	m_shader->setUniform1i("u_Texture", 0);
	m_shader->setUniform1i("u_RenderMode", static_cast<int>(RenderMode::COLOR));
	m_shader->setUniform1i("u_Light", 0);


	if (objectFileName != "elder.obj")
	{
		m_shader->setUniformbuffer("Materials", m_UBO->getRendererId());
	}
	m_objectState = true;
}

void ObjectRenderer::onRender()
{
	nrg::mat4 model(1.0f);

	Renderer renderer;
	m_texture->bind();
	model = nrg::translate(model, m_modelState.centerOffset);

	model = nrg::translate(model, m_modelState.translation); // move model. rotate have to be after move 
	model = nrg::rotate(model, nrg::radians(m_modelState.rotation_x), nrg::vec3(0.0f, 1.0f, 0.0f));
	model = nrg::rotate(model, nrg::radians(m_modelState.rotation_y), nrg::vec3(1.0f, 0.0f, 0.0f));

	model = nrg::translate(model, m_modelState.centerOffset * -1.0f);

	model = nrg::scale(model, m_modelState.scale);

	m_view = nrg::translate(nrg::mat4(1.0f), view_vec);


	nrg::mat4 mvp = m_proj * m_view * model;
	m_shader->bind();
	// TODO is not changes not to do
	m_shader->setUniformMat4f("u_MVP", mvp);
	m_shader->setUniformMat4f("u_model", model);
	m_shader->setUniform1i("u_RenderMode", static_cast<int>(m_modelState.renderMode));
	m_shader->setUniformVec3("u_lightPos", m_modelState.lightPos);
	m_shader->setUniform1i("u_Light", static_cast<int>(m_modelState.lightOn));
	m_shader->setUniform1i("u_isActive", static_cast<int>(m_isobjectActive));
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

nrg::vec3 ObjectRenderer::getObjectCenter()
{
	nrg::mat4 model(1.0f);
	nrg::vec4 localCenter = nrg::vec4(0.0f, 0.0f, 0.0f, 1.0f);

	model = nrg::translate(model, m_modelState.centerOffset);
	model = nrg::translate(model, m_modelState.translation); // move model. rotate have to be after move 
	model = nrg::translate(model, m_modelState.centerOffset * -1);
	model = nrg::scale(model, m_modelState.scale);
	m_view = nrg::translate(nrg::mat4(1.0f), view_vec);
	nrg::vec4 worldCenter =  model * localCenter;
	return nrg::vec3(worldCenter);
}
