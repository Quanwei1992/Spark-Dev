#pragma once
#include "Core/Base.h"
#include "Animation/Skeleton.h"

namespace Spark
{
	template<typename T>
	struct AnimationKey
	{
		T Value;
		float FrameTime;
		uint32_t Track;
		AnimationKey() = default;
		AnimationKey(float frameTime, uint32_t track, const T& value)
			: Value(value)
			, FrameTime(frameTime)
			, Track(track)

		{
		}
	};

	using TranslationKey = AnimationKey<Vector3>;
	using RotationKey = AnimationKey<Quat>;
	using ScaleKey = AnimationKey<Vector3>;

	class Animation
	{
	public:
		Animation() = default;
		Animation(std::string_view name,float duration);

		const std::string& GetName() const { return m_Name; }
		float GetDuration() const { return m_Duration; }

		void SetKeyFrames(const std::vector<TranslationKey>& translations, const std::vector<RotationKey>& rotations, const std::vector<ScaleKey>& scales);

		const auto& GetTranslationKeys() const { return m_Translations; }
		const auto& GetRotationKeys() const { return m_Rotations; }
		const auto& GetScaleKeys() const { return m_Scales; }


	private:
		std::string m_Name;
		float m_Duration = 0.0f;
		std::vector<TranslationKey> m_Translations;
		std::vector<RotationKey> m_Rotations;
		std::vector<ScaleKey> m_Scales;
	};

}
