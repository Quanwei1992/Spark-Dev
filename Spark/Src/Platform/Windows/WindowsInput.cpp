#include "Input/Input.h"
#include <GLFW/glfw3.h>
#include "Core/Application.h"
#include "Core/Window.h"
namespace Spark
{
	bool Input::IsKeyPressed(KeyCode keycode)
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		const auto state = glfwGetKey(window, static_cast<int>(keycode));
		return state == GLFW_PRESS || state == GLFW_REPEAT;
	}
	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		const auto state = glfwGetMouseButton(window, static_cast<int>(button));
		return state == GLFW_PRESS;
	}

	Vector2 Input::GetMousePosition()
	{
		const auto window = static_cast<GLFWwindow*>(Application::Get().GetWindow().GetNativeWindow());
		double x, y;
		glfwGetCursorPos(window, &x, &y);
		return Vector2(static_cast<float>(x), static_cast<float>(y));
	}
}