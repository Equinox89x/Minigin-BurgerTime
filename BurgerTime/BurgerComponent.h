#pragma once
#include "Component.h"
#include "GameObject.h"
#include "Subject.h"
#include "Scene.h"
#include <map>

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
		BurgerComponent(Scene* scene) : m_Scene{scene} {};
		~BurgerComponent() = default;
		BurgerComponent(const BurgerComponent&) = delete;
		BurgerComponent(BurgerComponent&&) noexcept = delete;
		BurgerComponent& operator=(const BurgerComponent&) = delete;
		BurgerComponent& operator=(BurgerComponent&&) noexcept = delete;

		void Initialize() override;
		void UpdateRects();
		void Update() override;
		void Render() const;

		void SetState(BurgerState state) { m_BurgerState = state; };
		BurgerState GetState() { return m_BurgerState; };

		void HandleOverlap(SDL_Rect& otherRect);
		void HandleOverlap(GameObject* otherObj);
		void HandlePlatformOverlap(SDL_Rect& otherRect, bool isFinished = false);


	private:
		struct SDL_RectCompare {
			bool operator()(const SDL_Rect& rect1, const SDL_Rect& rect2) const {
				if (rect1.y < rect2.y) return true;
				if (rect1.y > rect2.y) return false;
				return rect1.x < rect2.x;
			}
		};


		Scene* m_Scene;
		BurgerState m_BurgerState{ BurgerState::STATIC };
		const float m_FallSpeed{ 100 };
		const float m_NoInterruptTimerDefault{ 0.35f };
		float m_NoInterruptTimer{ m_NoInterruptTimerDefault };
		std::map<SDL_Rect, bool, SDL_RectCompare> m_Rects{  };
	};
}

