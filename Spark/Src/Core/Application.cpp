#include "Core/Application.h"
#include "Core/Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Core/Log.h"
#include <GLFW/glfw3.h>
namespace Spark
{
	void Application::Run()
	{
		
		m_Window = Window::Create();
		m_Window->SetEventCallback([this](Event&e) 
		{
			return this->OnEvent(e);
		});
		

		while(m_Running)
		{
			m_Window->OnUpdate();
		}

		delete m_Window;
		m_Window = nullptr;
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>([this](WindowCloseEvent& e)
		{
			return this->OnWindowClosed(e);
		});
		SP_TRACE(e);
	}

	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
