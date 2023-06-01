#pragma once

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>


template<typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::vec<L, T, Q>& vector)
{
	return os << glm::to_string(vector);
}

template<typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, const glm::mat<C, R, T, Q>& matrix)
{
	return os << glm::to_string(matrix);
}

template<typename OStream, typename T, glm::qualifier Q>
inline OStream& operator<<(OStream& os, glm::qua<T, Q> quaternion)
{
	return os << glm::to_string(quaternion);
}

namespace Spark
{
	using Vector2 = glm::vec2;
	using Vector3 = glm::vec3;
	using Vector4 = glm::vec4;

	using Mat3 = glm::mat3;
	using Mat4 = glm::mat4;

	using Quat = glm::quat;
}


namespace Spark
{
	class Math
	{
	public:
		static bool DecomposeTransform(const Mat4& transform, Vector3& translation, Quat& rotation, Vector3& scale);
		static Vector3 Rotate(const Quat& quat, const Vector3& v);
		static Vector3 GetRayPlaneIntersection(Vector3 rayOrigin, Vector3 rayDirection, Vector3 planeNormal, float planeDistance);
	};
}



#include "Math/AABB.h"