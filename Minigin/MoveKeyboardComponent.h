#pragma once
#include <string>
#include <memory>
#include "TransformComponent.h"
#include "MathLib.h"
#include <map>

namespace dae
{
	class MoveKeyboardComponent final : public Component
	{
	public:

		MoveKeyboardComponent(glm::vec3 startPos) : m_StartPos{ startPos }, m_Movespeed { 0,0,0 } {}
		~MoveKeyboardComponent() {};
		MoveKeyboardComponent(const MoveKeyboardComponent& other) = delete;
		MoveKeyboardComponent(MoveKeyboardComponent&& other) = delete;
		MoveKeyboardComponent& operator=(const MoveKeyboardComponent& other) = delete;
		MoveKeyboardComponent& operator=(MoveKeyboardComponent&& other) = delete;

		void Update() override;
		virtual void FixedUpdate() override;
		void Render() const override;

		void SetMoveSpeed(const glm::vec3& movespeed);
		const glm::vec3 GetMoveSpeed() { return m_Movespeed; };

		void SetCanMove(MathLib::Side side, bool canMove) {
			m_Movement[side] = canMove; 
		};
		const std::map<MathLib::Side, bool>& GetCanMove() { return m_Movement; };


	private:
		void UpdatePos(float dt);
		glm::vec3 m_StartPos;
		glm::vec3 m_Movespeed;

		std::map<MathLib::Side, bool> m_Movement{
			  std::make_pair(MathLib::Side::Top, false),
			  std::make_pair(MathLib::Side::Left, false),
			  std::make_pair(MathLib::Side::Right, false),
			  std::make_pair(MathLib::Side::Bottom, false),
		};
	};
}