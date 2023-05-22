#include "Shader.h"
#include "Renderer/RendererAPI.h"
#include "RHI/OpenGL/OpenGLShader.h"

namespace Spark
{
	Ref<Shader> Shader::Create(const std::filesystem::path& filePath)
	{
		if (RendererAPI::Current() == RendererAPIType::OpenGL)
		{
			return CreateRef<OpenGLShader>(filePath);
		}
		SP_CORE_ASSERT(false, "Unsupport Renderer API");
		return nullptr;
	}
}
