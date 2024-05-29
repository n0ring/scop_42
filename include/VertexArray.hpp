#pragma once

#include "glad/glad.h"
#include <GLFW/glfw3.h>
#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"


class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void addBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout);
	void bind() const;
	void unbind() const;

private:
	unsigned int m_renderID;
};