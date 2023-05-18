#include "EntryPoint.h"
#include "Application.h"
#include "Log.h"

namespace Spark
{
	int engine_main(int argc, const char** argv)
	{
		Log::Init();
		Application* app = CreateApplication();
		app->Run();
		delete app;
		Log::Shutdown();
		return 0;
	}
}
