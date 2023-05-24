#include "Animation.h"

namespace Spark
{
	Animation::Animation(const std::string_view name,const float duration)
		:m_Name(name),m_Duration(duration)
	{
	}

	void Animation::SetKeyFrames(const std::vector<TranslationKey>& translations,
		const std::vector<RotationKey>& rotations, const std::vector<ScaleKey>& scales)
	{
		m_Translations = std::move(translations);
		m_Rotations = std::move(rotations);
		m_Scales = std::move(scales);
	}
}
