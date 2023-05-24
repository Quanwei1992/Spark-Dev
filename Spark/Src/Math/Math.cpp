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
}
