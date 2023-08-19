#pragma once
#include "Component.h"
#include <vector>
#include <map>
#include <Scene.h>
#include "GameObject.h"
#include "Subject.h"

namespace dae {
	class BurgerManagerComponent final : public Component, public Subject
	{
	public:
		BurgerManagerComponent(Scene* scene) : m_Scene{ scene } {};
		~BurgerManagerComponent() = default;
		BurgerManagerComponent(const BurgerManagerComponent&) = delete;
		BurgerManagerComponent(BurgerManagerComponent&&) noexcept = delete;
		BurgerManagerComponent& operator=(const BurgerManagerComponent&) = delete;
		BurgerManagerComponent& operator=(BurgerManagerComponent&&) noexcept = delete;

		//void Initialize() override;
		void Update() override;
		void HandleBurgerOverlap(std::map<std::string, GameObject*>& map, const std::shared_ptr<GameObject>& enemyHolder, SDL_Rect& charRect, const std::shared_ptr<GameObject>& opposer);
		void Render() const;

		bool CheckAreBurgersFinished();
		void DeleteItems();

		std::map<std::string, GameObject*> AddBurger(GameObject* pattyTop, GameObject* pattyBottom, GameObject* veggie, GameObject* burger);
		std::pair<SDL_Rect, GameObject*> AddPlatform(SDL_Rect rect, GameObject* go);
		std::pair<SDL_Rect, GameObject*> AddLadder(SDL_Rect rect, GameObject* go);
		std::pair<SDL_Rect, GameObject*> AddPlate(SDL_Rect rect, GameObject* go);

	private:
		Scene* m_Scene{};
		std::vector<std::map<std::string, GameObject*>> m_Burgers;
		std::vector<std::pair<SDL_Rect, GameObject*>> m_Platforms;
		std::vector<std::pair<SDL_Rect, GameObject*>> m_pPlates;
		std::vector<std::pair<SDL_Rect, GameObject*>> m_pLadders;
	};

}

