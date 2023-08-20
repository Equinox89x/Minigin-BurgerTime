#pragma once
#include "Component.h"
#include <Scene.h>

namespace dae {
	class FloatingScoreComponent: public Component
	{
	public:
		FloatingScoreComponent(std::string text, glm::vec3 pos) : m_Text{ text }, m_Pos{pos} {};
		~FloatingScoreComponent() = default;
		FloatingScoreComponent(const FloatingScoreComponent&) = delete;
		FloatingScoreComponent(FloatingScoreComponent&&) noexcept = delete;
		FloatingScoreComponent& operator=(const FloatingScoreComponent&) = delete;
		FloatingScoreComponent& operator=(FloatingScoreComponent&&) noexcept = delete;

		void Initialize() override;
		void Update() override;

	private:
		const std::string m_Text;
		glm::vec3 m_Pos;
		float m_DeathTimer{ 0.5f };
		float m_Speed{ 2000.f };
	};
}

