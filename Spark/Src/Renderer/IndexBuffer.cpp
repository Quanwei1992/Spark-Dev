#include "IndexBuffer.h"
#include "Renderer/RendererAPI.h"
#include "RHI/OpenGL/OpenGLIndexBuffer.h"

namespace Spark
{
	Ref<IndexBuffer> IndexBuffer::Create(const void* indices, uint32_t size)
	{
		if (RendererAPI::Current() == RendererAPIType::OpenGL)
		{
			return CreateRef<OpenGLIndexBuffer>(indices, size);
		}
		SP_CORE_ASSERT(false, "Unsupport Renderer API");
		return nullptr;
	}
}
