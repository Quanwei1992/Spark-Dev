#pragma once
#include "Renderer/RendererAPI.h"

namespace Spark
{
	class OpenGLRendererAPI :public RendererAPI
	{
	public:
		void Init() override;
		void Shutdown() override;
		void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
		void SetClearColor(float r, float g, float b, float a) override;
		void Clear() override;
		void DrawIndexed(int count) override;
	};
}