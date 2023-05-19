#include "SandboxLayer.h"

#include <Spark.h>
#include <iostream>

using namespace Spark;

class SandBoxApp : public Application
{
public:
	SandBoxApp(ApplicationCommandLineArgs args)
		:Application("Sandbox",args)
	{
		PushLayer(new SandBoxLayer());
	}
};

Application* Spark::CreateApplication(ApplicationCommandLineArgs args)
{
	return new SandBoxApp(args);
}


int main(int argc,const char** argv)
{
	return engine_main(argc, argv);
}