#pragma once
#include "Component.h"
#include <glm/ext/vector_float2.hpp>
#include <GalagaMath.h>
#include <Scene.h>
#include <TextureComponent.h>

namespace dae {
	class EnemyComponent final : public Component
	{

	public:
		EnemyComponent(Scene* scene, int score) :
			m_Scene{ scene },
			Score{ score }
		{
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
		int GetLives() { return Lives; };
		void Damage() { Lives--; };
		void SetLives(int lives) { Lives = lives; };


	private:
		Scene* m_Scene;
		bool CanDie{ false };
		float DeathTimer{ .4f };
		int Lives{ 1 };
		int Score{};
	};

}
