#pragma once

#include <memory>
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "ParsedObject.hpp"


#include "nrg_math.hpp"
#include "ModelState.hpp"

class ObjectRenderer
{
public:
	ObjectRenderer(const std::string& objectFileName, const std::string& shaderFileName, const std::string& textureFileName);
	~ObjectRenderer() {
	}
	void onUpdate(float deltaTime);
	void onRender();
	void onImGuiRender(int& fill);

	nrg::vec3 getObjectCenter();
	inline ModelState& getModelState() {return m_modelState;}
	inline bool isObjectValid() {return m_objectState; }


private:
	std::unique_ptr<VertexArray> m_VAO;
	std::unique_ptr<IndexBuffer> m_IBO;
	std::unique_ptr<Shader> m_shader;
	std::unique_ptr<Texture> m_texture;
	std::unique_ptr<VertexBuffer> m_VBO;

	nrg::vec3 m_translation;
	nrg::mat4 m_proj, m_view;
	

	nrg::vec3 view_vec;
	ModelState m_modelState;
	std::unique_ptr<ParsedObject> m_parsedObject;
	bool m_objectState = false;
};