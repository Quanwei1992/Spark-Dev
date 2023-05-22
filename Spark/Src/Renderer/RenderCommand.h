#pragma once
#include "Core/Base.h"
#include "Renderer/RendererAPI.h"
namespace Spark
{
	class RenderCommand
	{
	public:
		static void Init();
		static void Shutdown();

		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
		{
			s_CurrentRendererAPI->SetViewport(x, y, width, height);
		}
		static void SetClearColor(float r, float g, float b, float a=1.0f)
		{
			s_CurrentRendererAPI->SetClearColor(r, g, b, a);
		}
		static void Clear()
		{
			s_CurrentRendererAPI->Clear();
		}
		static void DrawIndexed(int count)
		{
			s_CurrentRendererAPI->DrawIndexed(count);
		}

	private:
		static RendererAPI* s_CurrentRendererAPI;
	};
}
