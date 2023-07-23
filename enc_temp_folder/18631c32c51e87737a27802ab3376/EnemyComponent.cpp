#include "EnemyComponent.h"
#include "TransformComponent.h"
#include <Timer.h>
#include <random>
#include <Minigin.h>
#include <glm/gtx/rotate_vector.hpp>
#include "Renderer.h"
#include "PlayerComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include "AudioComponent.h"
#include "GameObject.h"

void dae::EnemyComponent::Initialize()
{
	
}

void dae::EnemyComponent::Update()
{
	float deltaTime{ Timer::GetInstance().GetDeltaTime() };
	//GetGameObject()->GetTransform()->AddTranslate(deltaTime * m_Speed, 0);

	float speed{ deltaTime * m_Speed };

	switch (m_State)
	{
	case dae::State::MovingUp:
		GetGameObject()->GetTransform()->AddTranslate(0, -speed);
		break;
	case dae::State::MovingLeft:
		GetGameObject()->GetTransform()->AddTranslate(-speed, 0);
		break;
	case dae::State::MovingRight:
		GetGameObject()->GetTransform()->AddTranslate(speed, 0);
		break;
	case dae::State::MovingDown:
		GetGameObject()->GetTransform()->AddTranslate(0, speed);
		break;
	case dae::State::Dying:
		break;
	default:
		break;
	}

}

bool dae::EnemyComponent::IsSameRect(const SDL_Rect& rect1, const SDL_Rect& rect)
{
	return (rect1.x == rect.x &&
		rect1.y == rect.y &&
		rect1.w == rect.w &&
		rect1.h == rect.h);
}

void dae::EnemyComponent::CheckMovement(const std::vector<std::pair<SDL_Rect, GameObject*>>& platforms, const std::vector<std::pair<SDL_Rect, GameObject*>>& ladders) {
	m_Rect = GetGameObject()->GetComponent<TextureComponent>()->GetRect();

	const auto xLeft{ m_Rect.x };
	const auto xRight{ m_Rect.x + m_Rect.w };
	m_BottomLeft = SDL_Rect{ xLeft, m_Rect.y + m_Rect.h, m_Rect.w / 5, m_Rect.h / 5 };
	m_BottomRight = SDL_Rect{ xRight, m_Rect.y + m_Rect.h, m_Rect.w / 5, m_Rect.h / 5 };
	m_BottomRight.x -= m_BottomRight.w;


	m_LeftUp = SDL_Rect{ xLeft, m_Rect.y, m_Rect.w / 5, m_Rect.h / 5 };
	m_LeftDown = SDL_Rect{ xLeft, m_Rect.y, m_Rect.w / 5, m_Rect.h / 5 };
	m_LeftUp.x -= m_LeftUp.w;
	m_LeftDown.x -= m_LeftDown.w;
	m_LeftDown.y += m_Rect.h - m_LeftDown.h;
	bool canMoveRight{ false };
	bool canMoveLeft{ false };
	bool canMoveUp{ false };
	bool canMoveDown{ false };

	m_CanChangeState = false;


	//TODO: check if you reached end of platform
	//only try to change state when a new rectangle is detected and when it overlaps
	for (auto item : platforms)
	{
		if (!IsSameRect(m_LastHor, item.first)) {
			if (MathLib::IsOverlapping(m_BottomRight, item.first)) {
				canMoveRight = true;
				m_LastHor = item.first;
				m_CanChangeState = true;
			}
			if (MathLib::IsOverlapping(m_BottomLeft, item.first)) {
				canMoveLeft = true;
				m_LastHor = item.first;
				m_CanChangeState = true;

			}
		}
	}

	for (auto item : ladders) {
		if (!IsSameRect(m_LastVert, item.first)) {
			if (MathLib::IsOverlapping(m_LeftUp, item.first)) {
				canMoveUp = true;
				m_LastVert = item.first;
				m_CanChangeState = true;
			}
			if (MathLib::IsOverlapping(m_LeftDown, item.first)) {
				canMoveDown = true;
				m_LastVert = item.first;
				m_CanChangeState = true;
			}
		}
	}
	
	if (m_CanChangeState) {

		//TODO: Calcualte chance of willing to move left or right when coming from a ladder, or to continue
		if (canMoveLeft && canMoveRight) {
			if (MathLib::CalculateChance() <= 0.5f) {
				m_State = State::MovingLeft;
			}
			else {
				m_State = State::MovingRight;
			}
		}
		else {
			if (canMoveLeft) {
				m_State = State::MovingLeft;
			}
			if (canMoveRight) {
				m_State = State::MovingRight;
			}
		}


		//TODO: Calcualte chance of willing to move up or down when coming from a platform, or to continue
		if (canMoveDown && canMoveUp) {
			if (MathLib::CalculateChance() <= 0.5f) {
				m_State = State::MovingDown;
			}
			else {
				m_State = State::MovingUp;
			}
		}
		else {
			if (canMoveDown) {
				m_State = State::MovingDown;
			}
			if (canMoveUp) {
				m_State = State::MovingUp;
			}
		}
	}
}

void dae::EnemyComponent::Render() const
{
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomLeft);
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 0, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomRight);
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftUp);
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftDown);
}

void dae::EnemyComponent::DestroyEnemy()
{
	
}
