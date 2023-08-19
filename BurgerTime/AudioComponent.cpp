#include "AudioComponent.h"

void dae::AudioComponent::PostInitialize()
{
	m_pAudioService = Locator::getAudio();
	if (m_pAudioService == nullptr) return;

	m_BeamSoundId = m_pAudioService->LoadSound("../Data/Sound/TractorBeam.wav");
	m_ShootSoundId = m_pAudioService->LoadSound("../Data/Sound/ShootSound.wav");
	m_BeamCapturedSoundId = m_pAudioService->LoadSound("../Data/Sound/TractorBeamCaptured.wav");
	m_DeathSoundId = m_pAudioService->LoadSound("../Data/Sound/EnemyDeathSound.wav");
	m_DiveSoundId = m_pAudioService->LoadSound("../Data/Sound/DiveSound.wav");
	m_PlayerDeathSoundId = m_pAudioService->LoadSound("../Data/Sound/PlayerDeathSound.wav");
	m_MenuSoundId = m_pAudioService->LoadSound("../Data/Sound/MenuMusic.wav");
}

void dae::AudioComponent::Update()
{
	if (m_pAudioService == nullptr) {
		m_pAudioService = Locator::getAudio();
		if (m_pAudioService == nullptr) return;

		m_BeamSoundId = m_pAudioService->LoadSound("../Data/Sound/TractorBeam.wav");
		m_ShootSoundId = m_pAudioService->LoadSound("../Data/Sound/ShootSound.wav");
		m_BeamCapturedSoundId = m_pAudioService->LoadSound("../Data/Sound/TractorBeamCaptured.wav");
		m_DeathSoundId = m_pAudioService->LoadSound("../Data/Sound/EnemyDeathSound.wav");
		m_DiveSoundId = m_pAudioService->LoadSound("../Data/Sound/DiveSound.wav");
		m_PlayerDeathSoundId = m_pAudioService->LoadSound("../Data/Sound/PlayerDeathSound.wav");
		m_MenuSoundId = m_pAudioService->LoadSound("../Data/Sound/MenuMusic.wav");
		PlayMenuSound();
	}
}

void dae::AudioComponent::PlayPlayerDeathSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_PlayerDeathSoundId);
}

void dae::AudioComponent::PlayDeathSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_DeathSoundId);
}

void dae::AudioComponent::PlayShootSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_ShootSoundId);
}

void dae::AudioComponent::PlayDiveSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_DiveSoundId);
}

void dae::AudioComponent::PlayTractorBeamSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_BeamSoundId);
}

void dae::AudioComponent::PlayCapturedTractorBeamSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->SetEffectVolume(60);
	m_pAudioService->PlaySound(m_BeamCapturedSoundId);
}

void dae::AudioComponent::PlayMenuSound(bool shouldStopPreviousSound)
{
	if (shouldStopPreviousSound) m_pAudioService->StopSound();
	m_pAudioService->PlaySound(m_MenuSoundId);
	m_pAudioService->SetEffectVolume(60);
}

void dae::AudioComponent::StopSound()
{
	m_pAudioService->StopSound();
}
