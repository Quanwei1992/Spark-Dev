#pragma once

#include "Renderer/GraphicsContext.h"

struct GLFWwindow;
namespace Spark
{
	class OpenGLContext : public GraphicsContext
	{
	public:
		OpenGLContext(GLFWwindow* windowHandle);
		~OpenGLContext() override;
		void Init() override;
		void SwapBuffers() override;
	private:
		GLFWwindow* m_WindowHandle;
	};
}