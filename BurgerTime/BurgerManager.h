#pragma once
#include "Component.h"
#include <vector>
#include <map>
#include <Scene.h>
#include "GameObject.h"


namespace dae {
	class BurgerManager final : public Component
	{
	public:
		BurgerManager(Scene* scene) : m_Scene{ scene } {};
		~BurgerManager() = default;
		BurgerManager(const BurgerManager&) = delete;
		BurgerManager(BurgerManager&&) noexcept = delete;
		BurgerManager& operator=(const BurgerManager&) = delete;
		BurgerManager& operator=(BurgerManager&&) noexcept = delete;

		//void Initialize() override;
		void Update() override;
		void Render() const;

		bool GetAreBurgersFinished();

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

