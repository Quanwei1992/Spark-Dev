#include "VertexArray.h"
#include "Renderer/RendererAPI.h"
#include "RHI/OpenGL/OpenGLVertexArray.h"

namespace Spark
{
	Ref<VertexArray> VertexArray::Create()
	{
		if(RendererAPI::Current() == RendererAPIType::OpenGL)
		{
			return CreateRef<OpenGLVertexArray>();
		}
		SP_CORE_ASSERT(false,"Unsupport Renderer API");
		return nullptr;
	}
}
