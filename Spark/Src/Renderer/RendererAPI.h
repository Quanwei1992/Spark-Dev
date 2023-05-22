#pragma once
#include "Core/Base.h"
namespace Spark
{
	enum class RendererAPIType
	{
		None,
		OpenGL
	};

	class RendererAPI
	{
	public:
		virtual ~RendererAPI() = default;

		virtual void Init() = 0;
		virtual void Shutdown() = 0;

		virtual  void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
		virtual void SetClearColor(float r, float g, float b, float a = 1.0f) = 0;
		virtual void Clear() = 0;
		virtual void DrawIndexed(int count) = 0;

		static RendererAPIType Current() { return s_CurrentRendererAPI; }

	private:
		inline static  RendererAPIType s_CurrentRendererAPI = RendererAPIType::OpenGL;
	};

}
