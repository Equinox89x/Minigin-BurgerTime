#pragma once
#include "Component.h"
#include <glm/ext/vector_float2.hpp>
#include <MathLib.h>
#include <Scene.h>
#include <TextureComponent.h>

namespace dae {

	enum class State {
		MovingUp,
		MovingLeft,
		MovingRight,
		MovingDown,
		Dying
	};

	class EnemyComponent final : public Component
	{

	public:
		EnemyComponent(Scene* scene, int score) :
			m_Scene{ scene },
			Score{ score }
		{};

		~EnemyComponent() = default;
		EnemyComponent(const EnemyComponent&) = delete;
		EnemyComponent(EnemyComponent&&) noexcept = delete;
		EnemyComponent& operator=(const EnemyComponent&) = delete;
		EnemyComponent& operator=(EnemyComponent&&) noexcept = delete;

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() const override;
		//virtual void FixedUpdate() override; 

		void DestroyEnemy();
		int GetLives() { return Lives; };
		void Damage() { Lives--; };
		void SetLives(int lives) { Lives = lives; };

		void CheckMovement(const std::vector<std::pair<SDL_Rect, GameObject*>>& platforms, const std::vector<std::pair<SDL_Rect, GameObject*>>& ladders);

	private:
		Scene* m_Scene;
		//bool CanDie{ false };
		float DeathTimer{ .4f };
		int Lives{ 1 };
		int Score{};
		int m_Speed{ 100 };
		bool m_CanChangeState{ false };

		SDL_Rect m_LeftDown, m_LeftUp, m_Rect, m_BottomLeft, m_BottomRight, m_LastVert, m_LastHor;
		State m_State{ State::MovingRight };

		bool IsSameRect(const SDL_Rect& rect1, const SDL_Rect& rect);
	};

}
