#pragma once

#include "Core/Base.h"
#include "Events/Event.h"

namespace Spark
{
	struct WindowProps
	{
		std::string Title;
		uint32_t Width;
		uint32_t Height;
		
		WindowProps(const std::string& title = "Spark",
			uint32_t width = 640,
			uint32_t height = 480)
			:Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		// Window attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVsync(bool enabled) = 0;
		virtual bool IsVsync() const = 0;

		virtual void* GetNativeWindow() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());

	};
}
