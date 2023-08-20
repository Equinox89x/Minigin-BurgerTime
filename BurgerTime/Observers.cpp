#include "Observers.h"
#include "ValuesComponent.h"
#include <string>
#include "Minigin.h"
#include "Observer.h"
#include "FileReader.h"
#include "InputManager.h"
#include "../BurgerTime/AudioComponent.h"
#include "BurgerManagerComponent.h"
#include <TextObjectComponent.h>
#include <TextureComponent.h>
#include "PlayerComponent.h"
#include "Commands.h"

void dae::HealthObserver::Notify(GameObject* go, Event& event)
{
	auto kids{ m_Scene->GetGameObject(EnumStrings[ScoreHolder])->GetChildren(EnumStrings[Life]) };
	auto i{ go->GetComponent<ValuesComponent>()->GetLives() };
	switch (event.GetEvent())
	{
	case EventType::Live:
		if (i >= 0) {
			kids[i]->SetIsHidden(true);
		}
		break;
	case EventType::Reset:
		break;
	}
}

void dae::ScoreObserver::Notify(GameObject* go, Event& event)
{
	ValuesComponent* comp{ go->GetComponent<ValuesComponent>() };
	auto score{ comp->GetScores() };
	switch (event.GetEvent())
	{
	case EventType::Score:
		if (auto child{ m_pGO->GetGameObject(EnumStrings[Names::Score]) })
			child->GetComponent<TextObjectComponent>()->SetText(std::to_string(score));
		break;
	case EventType::Reset:
		if (auto child{ m_pGO->GetGameObject(EnumStrings[Names::Score]) })
			child->GetComponent<TextObjectComponent>()->SetText(std::to_string(score));
		break;

	}
}

void dae::GameOverObserver::Notify(GameObject* go, Event& event)
{
	auto players{ m_Scene->GetGameObjects(EnumStrings[PlayerGeneral], false) };
	auto menu{ m_Scene->GetGameObject(EnumStrings[MainMenu]) };
	auto scoreboard{ m_Scene->GetGameObject(EnumStrings[ScoreHolder]) };
	auto enemies{ m_Scene->GetGameObject(EnumStrings[EnemyHolder]) };
	auto opposer{ m_Scene->GetGameObject(EnumStrings[Opposer]) };
	//auto values{ m_Scene->GetGameObject(EnumStrings[Values]) };
	auto burgerManager{ m_Scene->GetGameObject(EnumStrings[BurgerManager]) };
	auto selector{ m_Scene->GetGameObject(EnumStrings[Selector]) };



	switch (event.GetEvent())
	{
	case EventType::GameOver:

		//if (go->GetComponent<BurgerManagerComponent>()->CheckAreBurgersFinished()) {
			MakeEndScreen(m_Scene);
			scoreboard->GetComponent<ValuesComponent>()->GameEnd();
			Input::GetInstance().ClearKeys();

			for (auto object : go->GetChildren()) {
				object->MarkForDestroy();
			}

			scoreboard->MarkForDestroy();

			for (auto player : players) {
				player->MarkForDestroy();
			}
			if (enemies)enemies->MarkForDestroy();
			if (opposer)opposer->MarkForDestroy();
			//if (values)values->MarkForDestroy();

			if (burgerManager) {
				burgerManager->GetComponent<BurgerManagerComponent>()->DeleteItems();
				m_Scene->Remove(burgerManager);
			}

			menu->SetIsHidden(false);

			Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_UP, 0 }, std::make_unique<CycleGameMode>(selector.get(), true));
			Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_DOWN, 0 }, std::make_unique<CycleGameMode>(selector.get(), false));
			Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_SPACE, 0 }, std::make_unique<StartGame>(selector.get()));			
			
			Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, ControllerButton::ButtonY, 0 }, std::make_unique<CycleGameMode>(selector.get(), true));
			Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, ControllerButton::ButtonA, 0 }, std::make_unique<CycleGameMode>(selector.get(), false));
			Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, ControllerButton::ButtonX, 0 }, std::make_unique<StartGame>(selector.get()));

			//todo re-enable menu controls
		//}

		break;
	case EventType::Reset:
		break;

	}
}

void dae::StageClearedObserver::Notify(GameObject* /*go*/, Event& event)
{
	auto scoreboard{ m_pScene->GetGameObject(EnumStrings[ScoreBoard]) };
	auto enemyHolder{ m_pScene->GetGameObject(EnumStrings[EnemyHolder]) };
	auto opposer{ m_pScene->GetGameObject(EnumStrings[Opposer]) };
	auto values{ m_pScene->GetGameObject(EnumStrings[Values]) };
	auto burgerManager{ m_pScene->GetGameObject(EnumStrings[BurgerManager]) };

	auto player{ m_pScene->GetGameObject(EnumStrings[Player0]) };
	auto scoreHolder{ m_pScene->GetGameObject(EnumStrings[ScoreHolder]) };

	

	switch (event.GetEvent())
	{
	case EventType::StageCleared:

		m_pScene->Remove(enemyHolder);

		if (burgerManager) {
			burgerManager->GetComponent<BurgerManagerComponent>()->DeleteItems();
		}

		if (scoreHolder) {
			auto children{ scoreHolder->GetChildren(EnumStrings[Life]) };
			if (children.size() > 0) {
				for (size_t i = 0; i < 3; i++)
				{
					children[i]->SetIsHidden(false);
				}
			}
		}

		if (burgerManager) {
			if (burgerManager->GetComponent<TextureComponent>()->GetName() == "Stage 3") {
				CreateStage(m_pScene, Stages::Stage1, false);
				player->GetComponent<PlayerComponent>()->SetStartPos(glm::vec2{ (WindowSizeX / 2) - (Margin), WindowSizeY - ((Margin * 3) + WindowBuffer) });
				player->GetComponent<PlayerComponent>()->Reposition();
			}
			else if (burgerManager->GetComponent<TextureComponent>()->GetName() == "Stage 1") {
				CreateStage(m_pScene, Stages::Stage2, false);
				player->GetComponent<PlayerComponent>()->SetStartPos(glm::vec2{ (WindowSizeX / 2) - (Margin - 10), WindowSizeY - ((Margin * 1.5f) + WindowBuffer + 5) });
				player->GetComponent<PlayerComponent>()->Reposition();
			}
			else if (burgerManager->GetComponent<TextureComponent>()->GetName() == "Stage 2") {
				CreateStage(m_pScene, Stages::Stage3, false);
				player->GetComponent<PlayerComponent>()->SetStartPos(glm::vec2{ (WindowSizeX / 2) - (Margin - 10), WindowSizeY - (WindowBuffer + 10) });
				player->GetComponent<PlayerComponent>()->Reposition();
			}
		}
		break;
	case EventType::Reset:
		break;

	}
}
//
//
//void dae::ToMenu::Notify(GameObject* /*go*/, Event& event)
//{
//	switch (event.GetEvent())
//	{
//	case EventType::ToMenu:
//		m_pScene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayMenuSound();
//
//		m_pScene->GetGameObject(EnumStrings[EndScreen])->MarkForDestroy();
//		CreateMenuInput(m_pScene);
//		break;
//	case EventType::Reset:
//		break;
//	}
//}

void dae::PepperObserver::Notify(GameObject* go, Event& event)
{
	ValuesComponent* comp{ go->GetComponent<ValuesComponent>() };
	auto score{ comp->GetSalt() };
	switch (event.GetEvent())
	{
	case EventType::Pepper:
		if (auto child{ m_Scene->GetGameObject(EnumStrings[Names::Pepper]) })
			child->GetComponent<TextObjectComponent>()->SetText(std::to_string(score));
		break;
	case EventType::Reset:
		if (auto child{ m_Scene->GetGameObject(EnumStrings[Names::Pepper]) })
			child->GetComponent<TextObjectComponent>()->SetText(std::to_string(score));
		break;

	}
}
