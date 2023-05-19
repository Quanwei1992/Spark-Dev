#include "OpenGLContext.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace Spark
{

	void GLAPIENTRY MessageCallback(GLenum source,
		GLenum type,
		GLuint id,
		GLenum severity,
		GLsizei length,
		const GLchar* message,
		const void* userParam)
	{
		if (type != GL_DEBUG_TYPE_ERROR) return;
		SP_CORE_ERROR("GL CALLBACK: {0} type = {1}, severity = {2}, message = {3}", (type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : ""),
			type, severity, message);
	}


	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
		:m_WindowHandle(windowHandle)
	{
		SP_ASSERT(windowHandle, "Window handle is null!");
	}

	OpenGLContext::~OpenGLContext()
	{
		
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);
		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
		SP_ASSERT(status, "Failed to initialize Glad!");

		SP_INFO("OpenGL Info:");
		SP_INFO(" Vendor: {0}", (const char*)glGetString(GL_VENDOR));
		SP_INFO(" Renderer: {0}", (const char*)glGetString(GL_RENDERER));
		SP_INFO(" Version: {0}", (const char*)glGetString(GL_VERSION));


		glEnable(GL_DEBUG_OUTPUT);
		glDebugMessageCallback(MessageCallback, 0);
		glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
		//glDebugMessageControl(GL_DEBUG_SOURCE_API, GL_DEBUG_SEVERITY_HIGH, GL_DONT_CARE, 0, nullptr, GL_TRUE);

	}
	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);
	}
}