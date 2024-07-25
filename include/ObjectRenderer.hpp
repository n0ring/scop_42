#pragma once

#include <memory>
#include "VertexBuffer.hpp"
#include "VertexArray.hpp"
#include "IndexBuffer.hpp"
#include "Texture.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "ParsedObject.hpp"
#include "UniformBuffer.hpp"

#include "nrg_math.hpp"
#include "ModelState.hpp"

class ObjectRenderer
{
public:
	ObjectRenderer(const std::string &objectFileName,
				   const unsigned int objId,
				   const std::string &shaderFileName = "res/shaders/Basic.shader",
				   const std::string &textureFileName = "res/texture/tex1.bmp");
	~ObjectRenderer()
	{
	}

	ObjectRenderer(ObjectRenderer &&other)
		: m_UBO(std::move(other.m_UBO)),
		  m_VAO(std::move(other.m_VAO)),
		  m_IBO(std::move(other.m_IBO)),
		  m_shader(std::move(other.m_shader)),
		  m_texture(std::move(other.m_texture)),
		  m_VBO(std::move(other.m_VBO)),
		  m_translation(std::move(other.m_translation)),
		  m_proj(std::move(other.m_proj)),
		  m_view(std::move(other.m_view)),
		  view_vec(std::move(other.view_vec)),
		  m_modelState(std::move(other.m_modelState)),
		  m_parsedObject(std::move(other.m_parsedObject)),
		  m_objectState(other.m_objectState),
		  m_isobjectActive(other.m_isobjectActive),
		  m_objId(other.m_objId) {}

	ObjectRenderer &operator=(ObjectRenderer &&other) noexcept
	{
		if (this != &other)
		{
			m_UBO = std::move(other.m_UBO);
			m_VAO = std::move(other.m_VAO);
			m_IBO = std::move(other.m_IBO);
			m_shader = std::move(other.m_shader);
			m_texture = std::move(other.m_texture);
			m_VBO = std::move(other.m_VBO);
			m_translation = std::move(other.m_translation);
			m_proj = std::move(other.m_proj);
			m_view = std::move(other.m_view);
			view_vec = std::move(other.view_vec);
			m_modelState = std::move(other.m_modelState);
			m_parsedObject = std::move(other.m_parsedObject);
			m_objectState = other.m_objectState;
			m_isobjectActive = other.m_isobjectActive;
			m_objId = other.m_objId;
		}
		return *this;
	}
	void onUpdate(float deltaTime);
	void onRender();
	void onImGuiRender(int &fill);

	nrg::vec3 getObjectCenter();
	inline ModelState &getModelState() { return m_modelState; }
	inline bool isObjectValid() { return m_objectState; }
	inline void setObjectAsActive() { m_isobjectActive = true;}
	inline void setObjectAsNotActive() { m_isobjectActive = false;}

private:
	std::unique_ptr<UniformBuffer> m_UBO;
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
	bool m_isobjectActive = false;
	unsigned int m_objId;
};