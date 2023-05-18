#pragma once

namespace Spark
{
	class Application
	{
	public:
		Application() = default;
		virtual  ~Application() = default;
		void Run();
	};

	Application* CreateApplication();
}
