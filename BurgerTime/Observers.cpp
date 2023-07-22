#include "Observers.h"
#include "ValuesComponent.h"
#include <string>
#include "Minigin.h"
#include "Observer.h"
#include "FileReader.h"
#include "InputManager.h"
#include "../BurgerTime/AudioComponent.h"
#include "BurgerManager.h"

void dae::HealthObserver::Notify(GameObject* go, Event& event)
{
	auto i{ go->GetComponent<ValuesComponent>()->GetLives() };
	switch (event.GetEvent())
	{
	case EventType::Live:
		if (i >= 0) {
			GameObjects[i]->SetIsHidden(true);
		}
		break;
	case EventType::Reset:
		break;
	}
}

//void dae::ScoreObserver::Notify(GameObject* go, Event& event)
//{
//	ValuesComponent* comp{ go->GetComponent<ValuesComponent>() };
//	auto score{ comp->GetScores() };
//	switch (event.GetEvent())
//	{
//	case EventType::Score:
//		m_pTextComponent->SetText(std::to_string(score));
//		break;
//	case EventType::Reset:
//		m_pTextComponent->SetText(std::to_string(comp->GetScores()));
//		break;
//
//	}
//}

void dae::GameOverObserver::Notify(GameObject* go, Event& event)
{
	std::vector<std::shared_ptr<GameObject>> players{ m_Scene->GetGameObjects(EnumStrings[PlayerGeneral], false) };
	/*auto player0{ m_Scene->GetGameObject("Player0") };
	auto player1{ m_Scene->GetGameObject("Player1") };*/
	//auto scoreboard{ m_Scene->GetGameObject(EnumStrings[ScoreBoard]) };
	//auto enemies{ m_Scene->GetGameObject(EnumStrings[EnemyHolder]) };
	//auto opposer{ m_Scene->GetGameObject(EnumStrings[Opposer]) };
	auto values{ m_Scene->GetGameObject(EnumStrings[Values]) };
	//auto logo{ m_Scene->GetGameObject(EnumStrings[Logo])->GetTransform() };

	switch (event.GetEvent())
	{
	case EventType::GameOver:

		if (go->GetComponent<BurgerManager>()->GetAreBurgersFinished()) {
			MakeEndScreen(m_Scene);
			values->GetComponent<ValuesComponent>()->GameEnd();
			Input::GetInstance().ClearKeys();

			for (auto object : go->GetChildren()) {
				object->MarkForDestroy();
			}

			/*for (auto player : players) {
				player->MarkForDestroy();
			}
			if (enemies)
				enemies->MarkForDestroy();
			if (opposer)
				opposer->MarkForDestroy();
			if (values)
				values->MarkForDestroy();*/
		}

		break;
	case EventType::Reset:
		break;

	}
}

void dae::StageCleared::Notify(GameObject* /*go*/, Event& event)
{
	auto players{ m_pScene->GetGameObjects(EnumStrings[PlayerGeneral], false) };
	//auto player1{ m_pScene->GetGameObject("Player1") };
	auto scoreboard{ m_pScene->GetGameObject(EnumStrings[ScoreBoard]) };
	auto enemyHolder{ m_pScene->GetGameObject(EnumStrings[EnemyHolder]) };
	auto opposer{ m_pScene->GetGameObject(EnumStrings[Opposer]) };
	auto values{ m_pScene->GetGameObject(EnumStrings[Values]) };
	auto logo{ m_pScene->GetGameObject(EnumStrings[Logo])->GetTransform() };

	switch (event.GetEvent())
	{
	case EventType::StageCleared:

		if (m_pScene->GetGameObject("Stage 1")) {
			CreateStage(m_pScene, Stages::Stage2, 3);
			m_pScene->GetGameObject("Stage 1")->SetName("Stage 2");
			//m_pScene->GetGameObject("Player0")->GetComponent<ValuesComponent>()->Reset();
		}
		else if (m_pScene->GetGameObject("Stage 2")) {
			CreateStage(m_pScene, Stages::Stage3, 3);
			m_pScene->GetGameObject("Stage 2")->SetName("Stage 3");
			//m_pScene->GetGameObject("Player0")->GetComponent<ValuesComponent>()->Reset();
		}
		else if (m_pScene->GetGameObject("Stage 3")) {

			MakeEndScreen(m_pScene);

			values->GetComponent<ValuesComponent>()->GameEnd();

			for (auto player : players) {
				player->MarkForDestroy();
			}

			/*if (player0)
				player0->MarkForDestroy();
			if (player1)
				player1->MarkForDestroy();*/
				//if(m_Scene->GetGameObject("Player1")) m_Scene->Remove(m_Scene->GetGameObject("Player1"));
			scoreboard->MarkForDestroy();
			m_pScene->GetGameObject("Stage 3")->SetName("Stage 1");

			logo->AddTranslate(0, WindowSizeY);

			if (values)
				values->MarkForDestroy();
		}
		m_pScene->Remove(enemyHolder);
		break;
	case EventType::Reset:
		break;

	}
}


void dae::ToMenu::Notify(GameObject* /*go*/, Event& event)
{
	switch (event.GetEvent())
	{
	case EventType::ToMenu:
		m_pScene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayMenuSound();

		m_pScene->GetGameObject(EnumStrings[EndScreen])->MarkForDestroy();
		CreateMenuInput(m_pScene);
		break;
	case EventType::Reset:
		break;
	}
}
