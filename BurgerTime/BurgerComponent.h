#pragma once
#include "Component.h"
#include "GameObject.h"

namespace dae {
	enum class BurgerState {
		FALLING,
		STATIC,
		FINISHED
	};

	class BurgerComponent final : public Component
	{
	public:
		BurgerComponent() {};
		~BurgerComponent() = default;
		BurgerComponent(const BurgerComponent&) = delete;
		BurgerComponent(BurgerComponent&&) noexcept = delete;
		BurgerComponent& operator=(const BurgerComponent&) = delete;
		BurgerComponent& operator=(BurgerComponent&&) noexcept = delete;

		//void Initialize() override;
		void Update() override;
		//void Render() const;

		void SetState(BurgerState state) { m_BurgerState = state; };

	private:
		BurgerState m_BurgerState{ BurgerState::STATIC };
		const float m_FallSpeed{ 100 };
	};
}

