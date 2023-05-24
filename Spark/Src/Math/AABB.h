#pragma once
#include "Core/Base.h"
namespace Spark
{
	struct AABB
	{
		Vector3 Min, Max;
		AABB():Min(0.0f),Max(0.0f){}

		AABB(const Vector3& min,const Vector3& max)
			:Min(min),Max(max){}
	};
}