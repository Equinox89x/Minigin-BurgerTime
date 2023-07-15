#pragma once
#include "Component.h"
#include <memory>
#include "../BurgerTime/PlayerComponent.h"

namespace dae
{
	class ChangeAnimComponent final : public Component
	{
	public:

		ChangeAnimComponent(GameObject* player) : m_Player{player} {}
		~ChangeAnimComponent() {};
		ChangeAnimComponent(const ChangeAnimComponent& other) = delete;
		ChangeAnimComponent(ChangeAnimComponent&& other) = delete;
		ChangeAnimComponent& operator=(const ChangeAnimComponent& other) = delete;
		ChangeAnimComponent& operator=(ChangeAnimComponent&& other) = delete;

		//void Update() override;
		//virtual void FixedUpdate() override;
		//void Render() const override;

		void SetTexture(const PlayerComponent::Movement);


	private:
		GameObject* m_Player;
	};
}