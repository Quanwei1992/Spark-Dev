#include "Core/Application.h"
#include "Events/ApplicationEvent.h"
#include "Core/Log.h"
namespace Spark
{
	void Application::Run()
	{
		WindowResizeEvent e(1920, 1080);
		SP_TRACE(e);

		SP_CORE_TRACE("Application Run");
	}
}
