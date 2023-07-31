#include "ModeSelector.h"
#include "TextObjectComponent.h"
#include "Transformcomponent.h"
#include "Minigin.h"
#include <GameObject.h>

void dae::ModeSelector::CycleGameMode(bool isMoveUp)
{
	if (!isMoveUp) {
		if (m_SelectedGameMode != GameMode::VERSUS) {
			m_SelectedGameMode = GameMode(static_cast<int>(m_SelectedGameMode) + 1);
		}
	}
	else {
		if (m_SelectedGameMode != GameMode::SOLO) {
			m_SelectedGameMode = GameMode(static_cast<int>(m_SelectedGameMode) - 1);
		}
	}

	auto selector{ GetGameObject() };
	auto pos{ selector->GetTransform()->GetPosition() };
	selector->GetTransform()->Translate(pos.x, 150 + Margin * static_cast<int>(m_SelectedGameMode), pos.z);
}

void dae::ModeSelector::StartGame(GameObject* pMenu)
{
	pMenu->SetIsHidden(true);

	switch (m_SelectedGameMode)
	{
	case dae::ModeSelector::GameMode::SOLO:
		CreateStage(m_pScene);
		CreateScore(m_pScene);
		CreateMainPlayer(m_pScene, false);
		break;
	case dae::ModeSelector::GameMode::COOP:
		CreateStage(m_pScene);
		CreateScore(m_pScene);
		CreateMainPlayer(m_pScene, false);
		CreateSecondaryPlayer(m_pScene);
		break;
	case dae::ModeSelector::GameMode::VERSUS:
		CreateScore(m_pScene);
		CreateVersusStage(m_pScene);
		CreateMainPlayer(m_pScene, true);
		break;
	default:
		break;
	}

}