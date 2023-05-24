#pragma once
#include "Core/Base.h"

namespace Spark
{
	class Skeleton
	{
	public:
		static constexpr uint32_t NullIndex = ~0;

	public:
		Skeleton() = default;
		Skeleton(uint32_t size);

		uint32_t AddBone(std::string name, uint32_t parentIndex, const Mat4& transform);
		uint32_t GetBoneIndex(std::string_view name) const;
		uint32_t GetParentBoneIndex(uint32_t boneIndex) const;

		uint32_t GetNumBones() const;
		const std::string& GetBoneName(uint32_t boneIndex) const;
		const auto& GetBoneNames() const { return m_BoneNames; }

		const auto& GetBoneTranslations()const { return m_BoneTranslations; }
		const auto& GetBoneRotations()const { return m_BoneRotations; }
		const auto& GetBoneScales ()const { return m_BoneScales; }

		bool operator==(const Skeleton& other) const;
		bool operator!=(const Skeleton& other) const { return !(*this == other); }

	private:
		std::vector<std::string> m_BoneNames;
		std::vector<uint32_t> m_ParentBoneIndices;

		std::vector<Vector3> m_BoneTranslations;
		std::vector<glm::quat> m_BoneRotations;
		std::vector<Vector3> m_BoneScales;

	};
}