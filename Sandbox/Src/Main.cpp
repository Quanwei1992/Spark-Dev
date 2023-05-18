#include <Spark.h>
#include <iostream>

using namespace Spark;

class SandBoxApp : public Application
{
	
};

Application* Spark::CreateApplication()
{
	return new SandBoxApp;
}


int main(int argc,const char** argv)
{
	return engine_main(argc, argv);
}