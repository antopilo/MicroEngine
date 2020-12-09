
#include "Renderer.h"
#include "VertexBuffer.h"
#include "Debug.h"

namespace Engine {
	VertexBuffer::VertexBuffer(const void* data, unsigned int size)
	{
		GLCall(glGenBuffers(1, &m_RendererID));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
		
		GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
	}

	void VertexBuffer::SetData(const void* data, unsigned int size) {
		GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
	}

	VertexBuffer::~VertexBuffer()
	{
		GLCall(glDeleteBuffers(1, &m_RendererID));
	}

	void VertexBuffer::Bind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
	}

	void VertexBuffer::Unbind() const
	{
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
	}
}
