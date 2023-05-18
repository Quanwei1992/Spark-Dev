#include "WindowsWindow.h"
#include "Core/Log.h"
#include "Core/Assert.h"
#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Events/KeyEvent.h"

#include <GLFW/glfw3.h>

namespace Spark
{
	static bool s_GLFWInitialized = false;

	Window* Window::Create(const WindowProps& props)
	{
		return new WindowsWindow(props);
	}

	void WindowsWindow::OnUpdate()
	{
		glfwPollEvents();
	}

	WindowsWindow::WindowsWindow(const WindowProps& props)
	{
		Init(props);
	}

	WindowsWindow::~WindowsWindow()
	{
		Shutdown();
	}

	void WindowsWindow::SetEventCallback(const EventCallbackFn& callback)
	{
		m_Data.EventCallback = callback;
	}

	void WindowsWindow::SetVsync(bool enabled)
	{
		glfwSwapInterval(enabled ? 1 : 0);
		m_Data.VSync = enabled;
	}

	bool WindowsWindow::IsVsync() const
	{
		return m_Data.VSync;
	}

	void WindowsWindow::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;
		SP_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);


		if (!s_GLFWInitialized)
		{
			int success = glfwInit();
			SP_ASSERT(success, "Could not initialize GLFW!");
			s_GLFWInitialized = true;
		}
		{
			glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
			glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
			glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

			m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);
		}



		glfwSetWindowUserPointer(m_Window, &m_Data);
		SetVsync(true);

		// Set GLFW callbacks
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowResizeEvent event(width, height);
				data.Width = width;
				data.Height = height;

				data.EventCallback(event);
			});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				WindowCloseEvent event;
				data.EventCallback(event);
			});

		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS:
				{
					KeyPressedEvent event(key, 0);
					data.EventCallback(event);

				}break;
				case GLFW_RELEASE:
				{
					KeyReleasedEvent event(key);
					data.EventCallback(event);

				}break;
				case GLFW_REPEAT:
				{
					KeyPressedEvent event(key, 1);
					data.EventCallback(event);

				}break;
				}
			});

		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				switch (action)
				{
				case GLFW_PRESS:
				{
					MouseButtonPressedEvent event(button);
					data.EventCallback(event);

				}break;
				case GLFW_RELEASE:
				{
					MouseButtonReleasedEvent event(button);
					data.EventCallback(event);

				}break;
				}
			});

		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseMovedEvent event((float)xpos, (float)ypos);
				data.EventCallback(event);
			});

		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
			{
				WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
				MouseScrolledEvent event((float)xoffset, (float)yoffset);
				data.EventCallback(event);
			});

	}

	void WindowsWindow::Shutdown() const
	{
		glfwDestroyWindow(m_Window);
	}

}
