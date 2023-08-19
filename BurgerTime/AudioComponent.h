#pragma once
#include "Component.h"
#include "Audio.h"
#include "Locator.h"
namespace dae {
	class AudioComponent final : public Component
	{
	public:
		AudioComponent() = default;
		~AudioComponent() = default;
		AudioComponent(const AudioComponent&) = delete;
		AudioComponent(AudioComponent&&) noexcept = delete;
		AudioComponent& operator=(const AudioComponent&) = delete;
		AudioComponent& operator=(AudioComponent&&) noexcept = delete;

		virtual void PostInitialize() override;
		virtual void Update() override;

		void PlayPlayerDeathSound(bool shouldStopPreviousSound = true);
		void PlayDeathSound(bool shouldStopPreviousSound = true);
		void PlayShootSound(bool shouldStopPreviousSound = true);
		void PlayDiveSound(bool shouldStopPreviousSound = true);
		void PlayTractorBeamSound(bool shouldStopPreviousSound = true);
		void PlayCapturedTractorBeamSound(bool shouldStopPreviousSound = true);
		void PlayMenuSound(bool shouldStopPreviousSound = true);
		void StopSound();

	private:
		Audio* m_pAudioService{ nullptr };

		int m_BeamSoundId{ 0 };
		int m_ShootSoundId{ 0 };
		int m_DiveSoundId{ 0 };
		int m_BeamCapturedSoundId{ 0 };
		int m_DeathSoundId{ 0 };
		int m_PlayerDeathSoundId{ 0 };
		int m_MenuSoundId{ 0 };

	};
}

