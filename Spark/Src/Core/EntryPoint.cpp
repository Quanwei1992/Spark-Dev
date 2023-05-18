#include "EntryPoint.h"
#include "Application.h"

namespace Spark
{
	int engine_main(int argc, const char** argv)
	{
		Application* app = CreateApplication();
		app->Run();
		delete app;
		return 0;
	}
}
