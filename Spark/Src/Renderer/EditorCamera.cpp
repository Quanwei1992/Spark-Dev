#include "Renderer/EditorCamera.h"
#include "Input/Input.h"
#include <glm/gtx/transform.hpp>
namespace Spark
{
	EditorCamera::EditorCamera()
	{
		UpdateCameraView();

	}


	void EditorCamera::Update(const Timestep ts)
	{
		const auto& mouse = Input::GetMousePosition();
		const auto& delta = mouse - m_InitialMousePosition;

		if(Input::IsMouseButtonPressed(MouseCode::ButtonRight) && !Input::IsKeyPressed(KeyCode::LeftAlt))
		{
			m_CameraMode = CameraControllMode::FLYCAM;
			const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
			const float speed = GetCameraSpeed();
			const auto& upDir = Vector3(0.0f, yawSign, 0.0f);
			const auto& forwardDir = GetForwardDirection();
			const auto& rightDir = GetRightDirection();
			if (Input::IsKeyPressed(KeyCode::Q))
			{
				m_PositionDelta -= ts.GetMilliseconds() * speed * upDir;
			}
			if (Input::IsKeyPressed(KeyCode::E))
			{
				m_PositionDelta += ts.GetMilliseconds() * speed * upDir;
			}
			if (Input::IsKeyPressed(KeyCode::S))
			{
				m_PositionDelta -= ts.GetMilliseconds() * speed * forwardDir;
			}
			if (Input::IsKeyPressed(KeyCode::W))
			{
				m_PositionDelta += ts.GetMilliseconds() * speed * forwardDir;
			}
			if (Input::IsKeyPressed(KeyCode::A))
			{
				m_PositionDelta -= ts.GetMilliseconds() * speed * rightDir;
			}
			if (Input::IsKeyPressed(KeyCode::D))
			{
				m_PositionDelta += ts.GetMilliseconds() * speed * rightDir;
			}

			const auto& rotationSpeed = RotationSpeed();
			float deltaYaw = -rotationSpeed.x * delta.x;
			float deltaPitch = -rotationSpeed.y * delta.y;

			Quat quat = Quat(Vector3(deltaPitch, deltaYaw, 0.0f));
			m_ForwardDirection = Math::Rotate(quat, m_ForwardDirection);
			m_UpDirection = Math::Rotate(quat, m_UpDirection);
			m_RightDirection = Math::Rotate(quat, m_RightDirection);
			m_LookAt = m_Position + m_ForwardDirection * GetDistance();

		}else if (Input::IsMouseButtonPressed(MouseCode::ButtonMiddle))
		{
			MousePan(mouse);
		}else if(Input::IsKeyPressed(KeyCode::LeftAlt) && Input::IsMouseButtonPressed(MouseCode::ButtonLeft))
		{
			MouseRotate(delta);
		}

		m_InitialMousePosition = mouse;

		m_Position += m_PositionDelta;
		UpdateCameraView();
	}


	float EditorCamera::GetCameraSpeed() const
	{
		float speed = m_NormalSpeed;
		if(Input::IsKeyPressed(KeyCode::LeftShift))
		{
			speed *= 5;
		}
		return glm::clamp(speed, MIN_SPEED, MAX_SPEED);
	}

	float EditorCamera::GetDistance() const
	{
		return glm::distance(m_Position, m_LookAt);
	}

	Vector3 EditorCamera::ScreenToWorldPoint(Vector3 screenPos) const
	{
		const float clipX = -1.0f + (screenPos.x / m_ViewportWidth) * 2.0f;
		const float clipY = -1.0f + (screenPos.y / m_ViewportHeight) * 2.0f;
		const auto& inversedProj = glm::inverse(GetProjectionMatrix());
		Vector4 viewPos = inversedProj * glm::vec4(clipX,clipY,0.95f ,1.0f);
		if(abs(viewPos.w) > 0.0f)
		{
			viewPos = viewPos / viewPos.w;
		}
		const Vector3 dir = glm::normalize(viewPos);

		const Vector3 intersection = Math::GetRayPlaneIntersection(Vector3(0.0f), dir, {0.0f,0.0f,-1.0f}, screenPos.z);

		const Vector3 worldPos = glm::inverse(GetViewMatrix()) * Vector4(intersection,1.0f);
		return worldPos;
	}

	Vector2 EditorCamera::WorldToScreenPoint(Vector3 worldPos) const
	{
		const auto& viewSpacePos = GetViewMatrix() * Vector4(worldPos, 1.0f);
		const auto& clipSpacePos = GetProjectionMatrix()* viewSpacePos;
		auto ndcSpacePos = clipSpacePos;
		if(abs(ndcSpacePos.w) > 0.0f)
		{
			ndcSpacePos = ndcSpacePos / ndcSpacePos.w;
		}
		float x = (ndcSpacePos.x + 1.0f) / 2.0f * m_ViewportWidth;
		float y = (ndcSpacePos.y + 1.0f) / 2.0f * m_ViewportHeight;
		return { x,y };
	}


	void EditorCamera::UpdateArcball(const Vector2& delta)
	{
		if(Input::IsMouseButtonPressed(MouseCode::ButtonMiddle))
		{
			MousePan(delta);
		}else if(Input::IsMouseButtonPressed(MouseCode::ButtonLeft))
		{
			MouseRotate(delta);
		}else if(Input::IsMouseButtonPressed(MouseCode::ButtonRight))
		{
			MouseZoom((delta.x + delta.y) * 0.1f);
		}
	}

	void EditorCamera::UpdateCameraView()
	{
		const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

		m_ViewMatrix = glm::lookAt(m_Position, m_LookAt, glm::vec3{ 0.f, yawSign, 0.f });

		m_PositionDelta  *= 0.3f;
	}

	const Mat4& EditorCamera::GetViewMatrix() const
	{
		return m_ViewMatrix;
	}

	void EditorCamera::SetPosition(const Vector3& position)
	{
		m_Position = position;
	}

	void EditorCamera::Focus(const Vector3& focusPoint)
	{
		m_LookAt = focusPoint;
		m_CameraMode = CameraControllMode::FLYCAM;
		UpdateCameraView();
	}

	void EditorCamera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;
	}

	Vector2 EditorCamera::RotationSpeed() const
	{
		float speedX =  glm::pi<float>() / (m_ViewportWidth + 0.0001f) * 0.8f;
		float speedY = glm::pi<float>() / (m_ViewportHeight + 0.0001f) * 0.8f;
		return { speedX,speedY };
	}


	float EditorCamera::ZoomSpeed() const
	{
		return GetDistance() / 0.5f;
	}

	void EditorCamera::MouseRotate(const Vector2& delta)
	{
		const float deltaAngleX = -(2 * glm::pi<float>() / m_ViewportWidth);
		const float deltaAngleY = -(glm::pi<float>() / m_ViewportHeight);
		const float xAngle = delta.x * deltaAngleX;
		float yAngle = delta.y * deltaAngleY;

		const float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;

		// Extra step to handle the problem when the camera direction is the same as the up vector
		const float cosAngle = glm::dot(GetForwardDirection(), GetUpDirection());
		if (cosAngle * yawSign > 0.99f)
			yAngle = 0.f;

		Mat4 rotationMatrixX { 1.0f };
		rotationMatrixX = glm::rotate(rotationMatrixX, xAngle, GetUpDirection());

		m_Position = (rotationMatrixX * Vector4(m_Position - m_LookAt, 1.0f)) + Vector4(m_LookAt,1.0f);

		Mat4 rotationMatrixY{ 1.0f };
		rotationMatrixY = glm::rotate(rotationMatrixY, yAngle, GetRightDirection());

		m_Position = (rotationMatrixY * Vector4(m_Position - m_LookAt, 1.0f)) + Vector4(m_LookAt, 1.0f);

		m_ForwardDirection = glm::normalize(m_LookAt - m_Position);
		m_RightDirection = glm::normalize(glm::cross(m_ForwardDirection, { 0,yawSign,0 }));
		m_UpDirection = glm::normalize(glm::cross(m_RightDirection, m_ForwardDirection));
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(SP_BIND_EVENT(OnMouseScroll));
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		MouseZoom(e.GetYOffset() * 0.1f);
		UpdateCameraView();
		return true;
	}

	void EditorCamera::MousePan(const Vector2& mouse)
	{
		const auto& lastPos = ScreenToWorldPoint({ m_InitialMousePosition.x,m_InitialMousePosition.y,GetDistance() });
		const auto& pos = ScreenToWorldPoint({ mouse.x,mouse.y,GetDistance() });
		const auto& deltaPos = pos - lastPos;
		const float zSign = GetForwardDirection().z > 0 ? 1.0 : -1.0f;
		m_Position += zSign * m_RightDirection * deltaPos;
		m_Position += m_UpDirection * deltaPos;
		m_LookAt += zSign * m_RightDirection * deltaPos;
		m_LookAt += m_UpDirection * deltaPos;
	}

	void EditorCamera::MouseZoom(float delta)
	{
		m_PositionDelta += delta * ZoomSpeed() * GetForwardDirection();
	}

	Vector3 EditorCamera::GetUpDirection() const
	{
		return m_UpDirection;
	}

	Vector3 EditorCamera::GetRightDirection() const
	{
		return m_RightDirection;
	}

	Vector3 EditorCamera::GetForwardDirection() const
	{
		return m_ForwardDirection;
	}
}
