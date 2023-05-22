#include "OpenGLIndexBuffer.h"
#include <glad/glad.h>
namespace Spark
{
	OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, uint32_t size)
		:m_Size(size)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, m_Size, indices, GL_STATIC_DRAW);
	}
	OpenGLIndexBuffer::~OpenGLIndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}

	void OpenGLIndexBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		m_Size = size;
		glNamedBufferSubData(m_RendererID, offset, m_Size, data);
	}

	void OpenGLIndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLIndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
	uint32_t OpenGLIndexBuffer::GetCount() const
	{
		return m_Size / sizeof(uint32_t);
	}
}