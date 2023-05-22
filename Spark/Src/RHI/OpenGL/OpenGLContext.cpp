#include "OpenGLContext.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Spark
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		SP_CORE_ASSERT(windowHandle, "Window handle is null!");
	}

	OpenGLContext::~OpenGLContext()
	{
		
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SP_CORE_ASSERT(status, "Failed to initialize Glad!");
	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}