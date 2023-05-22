#include "Renderer/RenderCommand.h"
#include "RHI/OpenGL/OpenGLRendererAPI.h"
namespace Spark
{
	RendererAPI* RenderCommand::s_CurrentRendererAPI = nullptr;
	void RenderCommand::Init()
	{
		SP_TRACE("RenderCommand::Init");
		if(RendererAPI::Current() == RendererAPIType::OpenGL)
		{
			s_CurrentRendererAPI = new OpenGLRendererAPI;
		}

		s_CurrentRendererAPI->Init();

	}

	void RenderCommand::Shutdown()
	{
		s_CurrentRendererAPI->Shutdown();
		SP_TRACE("RenderCommand::Shutdown");
	}
}
