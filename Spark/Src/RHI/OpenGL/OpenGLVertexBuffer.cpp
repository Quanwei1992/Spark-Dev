#include "OpenGLVertexBuffer.h"
#include <glad/glad.h>
namespace Spark
{
	namespace Utils
	{
		GLenum toGLenum(VertexBufferUsage usage)
		{
			switch (usage)
			{
			case VertexBufferUsage::Static: return GL_STATIC_DRAW;
			case VertexBufferUsage::Dynamic: return GL_DYNAMIC_DRAW;
			}
			SP_CORE_ASSERT(false, "Unkown vertex buffer usage.");
			return 0;
		}
	}


	OpenGLVertexBuffer::OpenGLVertexBuffer(const void* data, uint32_t size, VertexBufferUsage usage)
		:m_Size(size), m_Usage(usage)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, m_Size, data, Utils::toGLenum(m_Usage));
	}

	OpenGLVertexBuffer::OpenGLVertexBuffer(uint32_t size, VertexBufferUsage usage)
		:m_Size(size), m_Usage(usage)
	{
		glCreateBuffers(1, &m_RendererID);
		glNamedBufferData(m_RendererID, m_Size, nullptr, Utils::toGLenum(m_Usage));
	}

	OpenGLVertexBuffer::~OpenGLVertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);
	}
	void OpenGLVertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
	}
	void OpenGLVertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void OpenGLVertexBuffer::SetData(const void* data, uint32_t size, uint32_t offset)
	{
		m_Size = size;
		glNamedBufferSubData(m_RendererID, offset, m_Size, data);
	}

}