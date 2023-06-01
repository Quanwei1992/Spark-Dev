#pragma once

#include "Core/Base.h"

namespace Spark
{
	enum class ProjectionMatrixMode
	{
		Perspective = 0,
		Orthographic
	};

	class Camera
	{
	public:
		Camera();

		ProjectionMatrixMode GetProjectionMatrixMode() const { return m_ProjectionMatrixMode; }
		void SetProjectionMatrixMode(const ProjectionMatrixMode mode);

		// Orthographic
		void SetWidth(float value);
		float GetWidth() const { return m_OrthoWidth; }

		void SetHeight(float value);
		float GetHeight() const { return m_OrthoHeight; }

		// Perspective
		void SetFov(float fov);
		float GetFov() const { return m_PerspectiveFov; }

		void SetAspect(float value);
		float GetAspect() const { return m_PerspectiveAspect; }

		// Clipping Planes
		void SetNear(float value);
		float GetNear() const { return m_Near; }

		void SetFar(float value);
		float GetFar() const { return m_Far; }

		const Mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }

	private:
		void UpdateProjectionMatrix();

	private:
		float m_OrthoWidth = 1;
		float m_OrthoHeight = 1;
		float m_PerspectiveFov = 40.0f;
		float m_PerspectiveAspect = 1.777f;
		float m_Near = 0.2f;
		float m_Far = 1000.0f;
		ProjectionMatrixMode m_ProjectionMatrixMode = ProjectionMatrixMode::Perspective;
		Mat4 m_ProjectionMatrix = Mat4(1.0f);
	};
}	