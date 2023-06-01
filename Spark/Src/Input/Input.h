#pragma once
#include "Core/Base.h"
#include "Input/KeyCodes.h"
#include "Input/MouseCodes.h"

namespace Spark
{
	class Input
	{
	public:
		static bool IsKeyPressed(KeyCode keycode);
		static bool IsMouseButtonPressed(MouseCode button);
		static Vector2 GetMousePosition();
	};
}