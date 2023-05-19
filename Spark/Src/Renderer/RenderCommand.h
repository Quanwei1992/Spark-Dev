#pragma once
#include "Core/Base.h"
namespace Spark
{
	class RenderCommand
	{
	public:
		static void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height);
		static void SetClearColor(float r, float g, float b, float a=1.0f);
		static void Clear();
		static void DrawIndexed(int count);
	};
}
