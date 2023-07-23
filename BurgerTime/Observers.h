#pragma once
#include "Observer.h"
#include "vector"
#include "Event.h"
#include "Scene.h"
#include "GameObject.h"
#include "TransformComponent.h"
#include <functional>


namespace dae
{
	//class HealthObserver final : public Observer
	//{
	//public:
	//	HealthObserver(const std::vector<GameObject*> gameObjects, Scene* scene) : GameObjects{ gameObjects }, m_Scene{ scene } {};
	//	void Notify(GameObject* go, Event& event) override;

	//private:
	//	const std::vector<GameObject*> GameObjects;
	//	Scene* m_Scene;
	//};

	//class GameOverObserver final : public Observer
	//{
	//public:
	//	GameOverObserver(std::function<void(Scene*)> createEndScreen, Scene* scene) : MakeEndScreen{ createEndScreen }, m_Scene{ scene } {};
	//	void Notify(GameObject* go, Event& event) override;

	//private:
	//	std::function<void(Scene* scene)> MakeEndScreen;
	//	Scene* m_Scene;
	//};


	//class StageCleared final : public Observer
	//{
	//public:
	//	StageCleared(std::function<void(Scene*, Stages, float)> createStage, std::function<void(Scene*)> makeEndScreen, Scene* scene) : CreateStage(createStage), MakeEndScreen{ makeEndScreen }, m_pScene{ scene } {}
	//	void Notify(GameObject* go, Event& event) override;

	//private:
	//	std::function<void(Scene*, Stages, float)> CreateStage;
	//	std::function<void(Scene*)> MakeEndScreen;
	//	Scene* m_pScene;
	//};

	//class ToMenu final : public Observer
	//{
	//public:
	//	ToMenu(GameObject* menu, std::function<void(Scene*)> createMenuInput, Scene* scene) : Menu{ menu }, CreateMenuInput{ createMenuInput }, m_pScene{ scene } {}
	//	void Notify(GameObject* go, Event& event) override;

	//private:
	//	GameObject* Menu;
	//	Scene* m_pScene;
	//	std::function<void(Scene*)> CreateMenuInput;
	//};

	class ScoreObserver final : public Observer
	{
	public:

		ScoreObserver(Scene* const scene) : m_pGO(scene) {
			//g_SteamAchievements = new CSteamAchievements(g_Achievements, 4);
		};
		void Notify(GameObject* go, Event& event) override;

	private:

		Scene* m_pGO;

	};
}