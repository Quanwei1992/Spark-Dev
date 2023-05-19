#include "Core/Application.h"
#include "Core/Window.h"
#include "Events/Event.h"
#include "Events/ApplicationEvent.h"
#include "Core/Log.h"
#include "Core/Time.h"

#include <GLFW/glfw3.h>

#include "Layer.h"

namespace Spark
{



	Application* Application::s_Instance = nullptr;

	Application::Application(const std::string& name, ApplicationCommandLineArgs args)
		: m_Name(name)
		, m_CommandLineArgs(args)
	{
		SP_ASSERT(!s_Instance, "Application already exsits!");
		s_Instance = this;

		WindowProps windowProps;
		windowProps.Title = name;
		m_Window = Window::Create(windowProps);
		m_Window->SetEventCallback(SP_BIND_EVENT(OnEvent));
	}

	Application::~Application()
	{
		delete m_Window;
	}

	void Application::Run()
	{

		while(m_Running)
		{
			const float time = static_cast<float>(Time::GetRealtimeSinceStartup());
			const Timestep timestep = time - m_LastFrameTime;
			m_LastFrameTime = time;

			if(!m_Minimized)
			{
				for (Layer* layer : m_Layers)
				{
					layer->OnUpdate(timestep);
				}
			}

			m_Window->OnUpdate();
		}
	}

	void Application::PushLayer(Layer* layer)
	{
		m_Layers.push_back(layer);
		layer->OnAttach();
	}


	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<WindowCloseEvent>(SP_BIND_EVENT(OnWindowClosed));
		dispatcher.Dispatch<WindowResizeEvent>(SP_BIND_EVENT(OnWindowResize));

		for (auto it = m_Layers.end(); it != m_Layers.begin();)
		{
			(*--it)->OnEvent(e);
			if (e.Handled) break;
		}
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0)
		{
			m_Minimized = true;
			return false;
		}
		m_Minimized = false;
		return false;
	}


	bool Application::OnWindowClosed(WindowCloseEvent& e)
	{
		m_Running = false;
		return true;
	}
}
