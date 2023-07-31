#include "Observers.h"
#include "ValuesComponent.h"
#include <string>
#include "Minigin.h"
#include "Observer.h"
#include "FileReader.h"
#include "InputManager.h"
#include "../BurgerTime/AudioComponent.h"
#include "BurgerManager.h"
#include <TextObjectComponent.h>

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
	std::vector<std::shared_ptr<GameObject>> players{ m_Scene->GetGameObjects(EnumStrings[PlayerGeneral], false) };
	auto scoreboard{ m_Scene->GetGameObject(EnumStrings[ScoreHolder]) };
	auto enemies{ m_Scene->GetGameObject(EnumStrings[EnemyHolder]) };
	auto opposer{ m_Scene->GetGameObject(EnumStrings[Opposer]) };
	auto values{ m_Scene->GetGameObject(EnumStrings[Values]) };

	switch (event.GetEvent())
	{
	case EventType::GameOver:

		if (go->GetComponent<BurgerManagerComponent>()->CheckAreBurgersFinished()) {
			MakeEndScreen(m_Scene);
			values->GetComponent<ValuesComponent>()->GameEnd();
			Input::GetInstance().ClearKeys();

			for (auto object : go->GetChildren()) {
				object->MarkForDestroy();
			}

			scoreboard->MarkForDestroy();

			for (auto player : players) {
				player->MarkForDestroy();
			}
			if (enemies)
				enemies->MarkForDestroy();
			if (opposer)
				opposer->MarkForDestroy();
			if (values)
				values->MarkForDestroy();
		}

		break;
	case EventType::Reset:
		break;

	}
}

void dae::StageClearedObserver::Notify(GameObject* /*go*/, Event& event)
{
	auto players{ m_pScene->GetGameObjects(EnumStrings[PlayerGeneral], false) };
	auto scoreboard{ m_pScene->GetGameObject(EnumStrings[ScoreBoard]) };
	auto enemyHolder{ m_pScene->GetGameObject(EnumStrings[EnemyHolder]) };
	auto opposer{ m_pScene->GetGameObject(EnumStrings[Opposer]) };
	auto values{ m_pScene->GetGameObject(EnumStrings[Values]) };
	auto burgerManager{ m_pScene->GetGameObject(EnumStrings[Names::BurgerManager]) };

	switch (event.GetEvent())
	{
	case EventType::StageCleared:

		m_pScene->Remove(enemyHolder);
		m_pScene->Remove(burgerManager);
		//TODO reset player location

		if (m_pScene->GetGameObject("Stage 1")) {
			CreateStage(m_pScene, Stages::Stage2);
			m_pScene->GetGameObject("Stage 1")->SetName("Stage 2");
		}
		else if (m_pScene->GetGameObject("Stage 2")) {
			CreateStage(m_pScene, Stages::Stage3);
			m_pScene->GetGameObject("Stage 2")->SetName("Stage 3");
		}
		else if (m_pScene->GetGameObject("Stage 3")) {

			MakeEndScreen(m_pScene);

			values->GetComponent<ValuesComponent>()->GameEnd();

			for (auto player : players) {
				player->MarkForDestroy();
			}

			scoreboard->MarkForDestroy();
			m_pScene->GetGameObject("Stage 3")->SetName("Stage 1");

			if (values)
				values->MarkForDestroy();
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
