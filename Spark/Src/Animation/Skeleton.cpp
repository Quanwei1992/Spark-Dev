#include "Animation/Skeleton.h"

namespace Spark
{
	Skeleton::Skeleton(uint32_t size)
	{
		m_BoneNames.reserve(size);
		m_ParentBoneIndices.reserve(size);
	}

	uint32_t Skeleton::AddBone(std::string name, uint32_t parentIndex, const Mat4& transform)
	{
		const uint32_t index = static_cast<uint32_t>(m_BoneNames.size());
		m_BoneNames.emplace_back(name);
		m_ParentBoneIndices.emplace_back(parentIndex);
		m_BoneTranslations.emplace_back();
		m_BoneRotations.emplace_back();
		m_BoneScales.emplace_back();
		Math::DecomposeTransform(transform, m_BoneTranslations.back(), m_BoneRotations.back(), m_BoneScales.back());
		return index;
	}

	uint32_t Skeleton::GetBoneIndex(std::string_view name) const
	{
		for (size_t i = 0; i < m_BoneNames.size(); ++i)
		{
			if (m_BoneNames[i] == name)
			{
				return static_cast<uint32_t>(i);
			}
		}
		return Skeleton::NullIndex;
	}

	bool Skeleton::operator==(const Skeleton& other) const
	{
		bool areSame = false;
		if(GetNumBones() == other.GetNumBones())
		{
			areSame = true;
			for (uint32_t i =0;i<GetNumBones();i++)
			{
				if(GetBoneName(i) != other.GetBoneName(i))
				{
					areSame = false;
					break;
				}
			}
		}
		return areSame;
	}

	uint32_t Skeleton::GetParentBoneIndex(uint32_t boneIndex) const
	{
		SP_CORE_ASSERT(boneIndex < m_ParentBoneIndices.size(), "Bone index out of range in Skeleton::GetParentBoneIndex()!");
		return m_ParentBoneIndices[boneIndex];
	}

	uint32_t Skeleton::GetNumBones() const
	{
		return m_BoneNames.size();
	}

	const std::string& Skeleton::GetBoneName(uint32_t boneIndex) const
	{
		SP_CORE_ASSERT(boneIndex < m_BoneNames.size(), "Bone index out of range in Skeleton::GetParentBoneIndex()!");
		return m_BoneNames[boneIndex];
	}


}
