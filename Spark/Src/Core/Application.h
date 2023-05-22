#pragma once

#include "Core/Timestep.h"
#include "Core/Assert.h"

#include <string>
#include <memory>
#include <vector>

namespace Spark
{

	struct ApplicationCommandLineArgs
	{
		int Count = 0;
		const char** Args = nullptr;

		const char* operator[](int index) const
		{
			SP_CORE_ASSERT(index < Count, "Out of range!");
			return Args[index];
		}
	};

	class Window;
	class Event;
	class WindowCloseEvent;
	class WindowResizeEvent;
	class Layer;

	class Application
	{
	public:
		Application(const std::string& name = "Spark", ApplicationCommandLineArgs args = ApplicationCommandLineArgs());
		virtual ~Application();
		void Run();
		void PushLayer(Layer* layer);

	private:
		void Init();
		void Shutdown();
		void OnEvent(Event& e);
		bool OnWindowResize(WindowResizeEvent& e);
		bool OnWindowClosed(WindowCloseEvent& e);
		std::string m_Name;
		ApplicationCommandLineArgs m_CommandLineArgs;
		Timestep m_Timestep;
		bool m_Minimized = false;
		float m_LastFrameTime = 0;
		Window* m_Window;
		bool m_Running = true;
		static Application* s_Instance;
		std::vector<Layer*> m_Layers;
	};

	Application* CreateApplication(ApplicationCommandLineArgs args);
}
