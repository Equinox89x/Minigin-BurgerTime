#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Subject.h"

namespace dae {
	enum class BurgerState {
		FALLING_NO_INTERUPT,
		FALLING,
		STATIC,
		FINISHED
	};

	class BurgerComponent final : public Component, public Subject
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
		BurgerState GetState() { return m_BurgerState; };

		void HandleOverlap(SDL_Rect& otherRect);
		void HandleOverlap(GameObject* otherObj);
		void HandlePlatformOverlap(SDL_Rect& otherRect, bool isFinished = false);


	private:
		BurgerState m_BurgerState{ BurgerState::STATIC };
		const float m_FallSpeed{ 100 };
		const float m_NoInterruptTimerDefault{ 0.35f };
		float m_NoInterruptTimer{ m_NoInterruptTimerDefault };
	};
}

