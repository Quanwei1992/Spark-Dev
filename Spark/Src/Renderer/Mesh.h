#pragma once

#include "Core/Base.h"
#include "Renderer/Submesh.h"
#include "Animation/Skeleton.h"
#include "Animation/Animation.h"

namespace Spark
{
	
	struct Vertex
	{
		Vector3 Position;
		Vector3 Normal;
		Vector3 Tangent;
		Vector3 Binormal;
		Vector2 Texcoord;
	};

	struct Index
	{
		uint32_t V1, V2, V3;
	};


	struct MeshNode
	{
		uint32_t Parent = ~0;
		std::vector<uint32_t> Children;
		std::vector<uint32_t> Submeshes;

		std::string Name;
		Mat4 LocalTransform;

		bool IsRoot() const { return Parent == ~0; }
	};

	struct BoneInfo
	{
		Mat4 SubmeshInverseTransform;
		Mat4 InverseBindPose;
		uint32_t SubMeshIndex;
		uint32_t BoneIndex;

		BoneInfo() = default;
		BoneInfo(const Mat4& subMeshInverseTransform,const Mat4& inverseBindPose,uint32_t subMeshIndex,uint32_t boneIndex)
			: SubmeshInverseTransform(subMeshInverseTransform)
			, InverseBindPose(inverseBindPose)
			, SubMeshIndex(subMeshIndex)
			, BoneIndex(boneIndex)
		{
			
		}
	};


	struct BoneInfluence
	{
		uint32_t BoneInfoIndices[4] = { 0,0,0,0 };
		float Weights[4] = {0.0f,0.0f,0.0f,0.0f};

		void AddBoneData(uint32_t boneInfoIndex, float weight);
		void NormalizeWeights();

	};

	class Mesh
	{
	public:
		Mesh(const std::filesystem::path& filepath);
		~Mesh();

		bool HasSkeleton() const { return m_Skeleton != nullptr; }

	private:
		void TraverseNodes(void* assimpNode, uint32_t nodeIndex, const Mat4& parentTransform = Mat4(1.0f), uint32_t level = 0);
	private:
		AABB m_BoundingBox;
		std::vector<Submesh> m_Submeshes;

		std::vector<Vertex> m_Vertices;
		std::vector<Index> m_Indices;

		Skeleton* m_Skeleton = nullptr;
		std::vector<Animation*> m_Animations;
		std::vector<BoneInfluence> m_BoneInfluences;
		std::vector<BoneInfo> m_BoneInfo;
		std::vector<MeshNode> m_Nodes;
	};

}