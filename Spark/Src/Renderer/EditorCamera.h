#pragma once

#include "Core/Base.h"
#include "Renderer/Camera.h"
#include "Core/Timestep.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
namespace Spark
{

	enum class CameraControllMode
	{
		NONE,
		FLYCAM,
		ARCBALL	
	};

	class EditorCamera : public Camera
	{
	public:
		EditorCamera();

		void Update(const Timestep ts);
		void OnEvent(Event& e);

		const Mat4& GetViewMatrix() const;
		void SetPosition(const Vector3& position);
		const Vector3& GetPosition() const { return m_Position; }

		void Focus(const Vector3& focusPoint);

		void SetViewportSize(uint32_t width, uint32_t height);

	private:
		void UpdateCameraView();
		bool OnMouseScroll(MouseScrolledEvent& e);

		void MousePan(const Vector2& delta);
		void MouseZoom(float delta);

		Vector2 RotationSpeed() const;
		float ZoomSpeed() const;

		void MouseRotate(const Vector2& delta);
		void UpdateArcball(const Vector2& delta);

		Vector3 GetUpDirection() const;
		Vector3 GetRightDirection() const;
		Vector3 GetForwardDirection() const;

		float GetCameraSpeed() const;

		float GetDistance() const;

		Vector3 ScreenToWorldPoint(Vector3 screenPos) const;
		Vector2 WorldToScreenPoint(Vector3 worldPos) const;

	private:

		Mat4 m_ViewMatrix  { 1.0f };
		Vector3 m_UpDirection { 0,1,0 };
		Vector3 m_RightDirection { 1,0,0 };
		Vector3 m_ForwardDirection{ 0,0,-1 };

		Vector3 m_Position { 0.0f };
		Vector3 m_LookAt{ 0.0f };

		bool m_Panning{ false };
		bool m_Rotating{ false };
		Vector2 m_InitialMousePosition{ 0.0f };
		Vector3 m_InitialFocalPoint{ 0.0f };
		Vector3 m_InitialRotation{ 0.0f };

		float m_NormalSpeed{ 0.01 };

		Vector3 m_PositionDelta {0.0f};

		CameraControllMode m_CameraMode{ CameraControllMode::ARCBALL };

		float m_MinFocusDistance {100.0f};

		uint32_t m_ViewportWidth{ 800 };
		uint32_t m_ViewportHeight{ 600 };

		constexpr static float MIN_SPEED{ 0.0005f }, MAX_SPEED{ 2.0f };
	};
}