#pragma once 

#include "VertexBuffer.hpp"
#include "VertexBufferLayout.hpp"
#include "Render.hpp"

class VertexArray
{
	public:
		VertexArray();
		~VertexArray();

		void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
		void bind() const;
		void unbind() const;
	private:
		unsigned int m_renderID;
};