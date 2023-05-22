#include "VertexBuffer.h"
#include "Renderer/RendererAPI.h"
#include "RHI/OpenGL/OpenGLVertexBuffer.h"

namespace Spark
{
	void BufferLayout::CalculateOffsetsAndStride()
	{
		uint32_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements)
		{
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	Ref<VertexBuffer> VertexBuffer::Create(const void* data, uint32_t size, VertexBufferUsage usage)
	{
		if (RendererAPI::Current() == RendererAPIType::OpenGL)
		{
			return CreateRef<OpenGLVertexBuffer>(data,size,usage);
		}
		SP_CORE_ASSERT(false, "Unsupport Renderer API");
		return nullptr;
	}

	Ref<VertexBuffer> VertexBuffer::Create(uint32_t size, VertexBufferUsage usage)
	{
		if (RendererAPI::Current() == RendererAPIType::OpenGL)
		{
			return CreateRef<OpenGLVertexBuffer>(size, usage);
		}
		SP_CORE_ASSERT(false, "Unsupport Renderer API");
		return nullptr;
	}
}
