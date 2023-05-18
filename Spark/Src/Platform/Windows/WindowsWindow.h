#pragma once

#include "Core/Base.h"
#include "Core/Window.h"

struct GLFWwindow;
namespace Spark
{
	class GraphicsContext;

	class WindowsWindow : public Window
	{
	public:
		WindowsWindow(const WindowProps& props);
		~WindowsWindow() override;

		void OnUpdate() override;
		unsigned int GetWidth() const override { return m_Data.Width; }
		unsigned int GetHeight() const override { return m_Data.Height; }
		void SetEventCallback(const EventCallbackFn& callback) override;
		void SetVsync(bool enabled) override;
		bool IsVsync() const override;
		void* GetNativeWindow() const override { return m_Window; }

	private:
		void Init(const WindowProps& props);
		void Shutdown() const;

		GLFWwindow* m_Window = nullptr;
		struct WindowData
		{
			std::string Title;
			unsigned int Width = 0, Height = 0;
			bool VSync = true;

			EventCallbackFn EventCallback;
		};
		WindowData m_Data;
	};
}