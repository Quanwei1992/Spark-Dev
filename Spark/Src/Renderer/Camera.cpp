#include "Renderer/Camera.h"

namespace Spark
{
	Camera::Camera()
	{
		UpdateProjectionMatrix();
	}

	void Camera::SetProjectionMatrixMode(const ProjectionMatrixMode mode)
	{
		if (mode == m_ProjectionMatrixMode) return;
		m_ProjectionMatrixMode = mode;
		UpdateProjectionMatrix();
	}

	void Camera::SetWidth(float value)
	{
		if (m_OrthoWidth == value) return;
		m_OrthoWidth = value;
		if (m_ProjectionMatrixMode == ProjectionMatrixMode::Orthographic)
		{
			UpdateProjectionMatrix();
		}
	}

	void Camera::SetHeight(float value)
	{
		if (m_OrthoHeight == value) return;
		m_OrthoHeight = value;
		if (m_ProjectionMatrixMode == ProjectionMatrixMode::Orthographic)
		{
			UpdateProjectionMatrix();
		}
	}


	void Camera::SetFov(float fov)
	{
		if (m_PerspectiveFov == fov) return;
		m_PerspectiveFov = fov;
		if(m_ProjectionMatrixMode == ProjectionMatrixMode::Perspective)
		{
			UpdateProjectionMatrix();
		}
	}

	void Camera::SetAspect(float value)
	{
		if (m_PerspectiveAspect == value) return;
		m_PerspectiveAspect = value;
		if(m_ProjectionMatrixMode == ProjectionMatrixMode::Perspective)
		{
			UpdateProjectionMatrix();
		}
	}

	void Camera::SetNear(float value)
	{
		if (m_Near == value) return;
		m_Near = value;
		UpdateProjectionMatrix();
			
	}

	void Camera::SetFar(float value)
	{
		if (m_Far == value) return;
		m_Far = value;
		UpdateProjectionMatrix();
	}

	void Camera::UpdateProjectionMatrix()
	{
		if(m_ProjectionMatrixMode == ProjectionMatrixMode::Orthographic)
		{
			const float halfWidth = m_OrthoWidth * 0.5f;
			const float halfHeight = m_OrthoHeight * 0.5f;
			m_ProjectionMatrix = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_Near, m_Far);
		}else if(m_ProjectionMatrixMode == ProjectionMatrixMode::Perspective)
		{
			m_ProjectionMatrix = glm::perspective(glm::radians(m_PerspectiveFov), m_PerspectiveAspect, m_Near, m_Far);
		}
	}
}
