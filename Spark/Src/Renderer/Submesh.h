#pragma once
#include "Core/Base.h"

namespace Spark
{
	struct Submesh
	{
		uint32_t BaseVertex;
		uint32_t BaseIndex;
		uint32_t IndexCount;
		uint32_t VertexCount;

		Mat4 Transform { 1.0f };
		Mat4 LocalTransform{ 1.0f };
		AABB BoundingBox;

		std::string NodeName, MeshName;

		bool IsRigged = false;
	};
}
