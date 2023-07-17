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
		//void Render() const;

		std::map<std::string, GameObject*> AddBurger(GameObject* pattyTop, GameObject* pattyBottom, GameObject* veggie, GameObject* burger);

	private:
		Scene* m_Scene{};
		std::vector<std::map<std::string, GameObject*>> m_Burgers;
	};

}

