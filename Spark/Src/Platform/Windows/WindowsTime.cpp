#include "Core/Time.h"
#include <GLFW/glfw3.h>
namespace Spark
{
	double Time::GetRealtimeSinceStartup()
	{
		return glfwGetTime();
	}
}

