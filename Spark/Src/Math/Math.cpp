#include "Math/Math.h"
#include <glm/gtx/matrix_decompose.hpp>
namespace Spark
{
	glm::vec3 Scale(const glm::vec3& v, float desiredLength)
	{
		return v * desiredLength / length(v);
	}

	bool Math::DecomposeTransform(const Mat4& transform, Vector3& translation, Quat& rotation, Vector3& scale)
	{
		glm::vec3 skew;
		glm::vec4 perspective;
		return glm::decompose(transform, scale, rotation, translation,skew,perspective);
	}

	Vector3 Math::Rotate(const Quat& quat, const Vector3& v)
	{
		const auto& rotationMatrix = glm::mat4_cast(quat);
		return rotationMatrix * Vector4(v, 1.0f);
	}

	Vector3 Math::GetRayPlaneIntersection(Vector3 rayOrigin, Vector3 rayDirection, Vector3 planeNormal,
		float planeDistance)
	{
		const float denominator = glm::dot(rayDirection, planeNormal);
		if (abs(denominator) < 1e-6)
		{
			return Vector3(0.0f, 0.0f, 0.0f);
		}
		const float t = (planeDistance - glm::dot(rayOrigin, planeNormal)) / denominator;
		return rayOrigin + t * rayDirection;
	}
}
