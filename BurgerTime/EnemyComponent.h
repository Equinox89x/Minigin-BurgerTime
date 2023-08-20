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

	enum class EnemyType{
		Hotdog, Pickle, Egg
	};

	class EnemyComponent final : public Component
	{

	public:
		EnemyComponent(Scene* scene, EnemyType enemyType, glm::vec2 startPos) :
			m_Scene{ scene },
			m_EnemyType{ enemyType },
			m_StartPos{ startPos }
		{
			switch (m_EnemyType)
			{
			case dae::EnemyType::Hotdog:
				m_EnemyTypeName = "hotdog";
				m_Score = 100;
				break;
			case dae::EnemyType::Pickle:
				m_EnemyTypeName = "pickle";
				m_Score = 200;
				break;
			case dae::EnemyType::Egg:
				m_EnemyTypeName = "egg";
				m_Score = 300;
				break;
			default:
				break;
			}
		};

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
		int GetLives() { return m_Lives; };
		void Damage() { m_Lives--; };
		void SetLives(int lives) { m_Lives = lives; };

		void CheckMovement(const std::vector<std::pair<SDL_Rect, GameObject*>>& platforms, const std::vector<std::pair<SDL_Rect, GameObject*>>& ladders);
		void CheckHit(GameObject* go);
		void Respawn();

	private:
		Scene* m_Scene;
		bool m_CanDie{ false };
		float m_DeathTimer{ .4f };
		int m_Lives{ 1 };
		int m_Score{};
		int m_Speed{ 100 };
		bool m_CanChangeState{ false };

		SDL_Rect m_LeftMapBorder{ 0,0, 10,770 }, m_RightMapBorder{ 760,0, 10,770 };
		SDL_Rect m_LeftDown, m_LeftUp, m_Rect, m_BottomLeft, m_BottomRight, m_LastVert, m_LastHor;
		State m_State{ State::MovingRight };
		EnemyType m_EnemyType{ EnemyType::Hotdog };
		std::string m_EnemyTypeName{ "hotdog" };
		glm::vec2 m_StartPos;


		bool IsSameRect(const SDL_Rect& rect1, const SDL_Rect& rect);
	};

}
