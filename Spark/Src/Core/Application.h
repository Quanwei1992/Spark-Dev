#pragma once

namespace Spark
{
	class Window;
	class Event;
	class WindowCloseEvent;

	class Application
	{
	public:
		Application() = default;
		virtual  ~Application() = default;

		void Run();

	private:

		void OnEvent(Event& e);
		bool OnWindowClosed(WindowCloseEvent& e);

		Window* m_Window;
		bool m_Running = true;
	};

	Application* CreateApplication();
}
