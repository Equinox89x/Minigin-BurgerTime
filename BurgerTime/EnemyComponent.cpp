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
#include <ValuesComponent.h>
#include "BurgerComponent.h"
#include "FloatingScoreComponent.h"

void dae::EnemyComponent::Initialize()
{
	
}

void dae::EnemyComponent::Update()
{
	float deltaTime{ Timer::GetInstance().GetDeltaTime() };

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
		m_DeathTimer -= deltaTime;
		if (m_DeathTimer <= 0) {
			Respawn();
		}
		break;
	case dae::State::Stunned:
		m_StunnedTimer -= deltaTime;
		if (m_StunnedTimer < 0) {
			m_StunnedTimer = m_DefaultStunnedTimer;
			m_State = m_PrevState;
		}
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
	m_LeftUp.y += m_Rect.h - int(m_LeftDown.h*1.5f);
	m_LeftDown.x -= m_LeftDown.w;
	m_LeftDown.y += m_Rect.h - m_LeftDown.h;
	bool canMoveRight{ false };
	bool canMoveLeft{ false };
	bool canMoveUp{ false };
	bool canMoveDown{ false };

	m_CanChangeState = false;


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
	
	if (m_State == State::MovingLeft || m_State == State::MovingRight) {
		if (!MathLib::IsOverlapping(m_BottomRight, m_LastHor)) {
			canMoveLeft = false;
			m_CanChangeState = true;
		}
		if (!MathLib::IsOverlapping(m_BottomLeft, m_LastHor)) {
			canMoveRight = false;
			m_CanChangeState = true;
		}

		
		if (MathLib::IsOverlapping(m_BottomLeft, m_LeftMapBorder)) {
			canMoveLeft = false;
			canMoveRight = true;
			m_CanChangeState = true;
		}
		if (MathLib::IsOverlapping(m_BottomRight, m_RightMapBorder)) {
			canMoveLeft = true;
			canMoveRight = false;
			m_CanChangeState = true;
		}
	}

	if (m_State == State::MovingUp || m_State == State::MovingDown) {
		if (!MathLib::IsOverlapping(m_LeftUp, m_LastVert)) {
			if (MathLib::IsOverlapping(m_LeftDown, m_LastVert)) {
				canMoveDown = true;
			}
			canMoveUp = false;
			m_CanChangeState = true;
		}
		if (!MathLib::IsOverlapping(m_LeftDown, m_LastVert)) {
			if (MathLib::IsOverlapping(m_LeftUp, m_LastVert)) {
				canMoveUp = true;
			}
			canMoveDown = false;
			m_CanChangeState = true;
			
		}
	}


	//change state depending on overlap results
	if (m_CanChangeState) {
		//TODO: Calcualte chance of willing to move left or right when coming from a ladder, or to continue
		if (canMoveLeft && canMoveRight) {
			if (MathLib::CalculateChance() <= 0.5f) {
				m_State = State::MovingLeft;
				GetGameObject()->GetComponent<TextureComponent>()->SetTexture(m_EnemyTypeName + "Left.png", 0.1f, 2);
			}
			else {
				m_State = State::MovingRight;
				GetGameObject()->GetComponent<TextureComponent>()->SetTexture(m_EnemyTypeName + "Right.png", 0.1f, 2);
			}
		}
		else {
			if (canMoveLeft) {
				m_State = State::MovingLeft;
				GetGameObject()->GetComponent<TextureComponent>()->SetTexture(m_EnemyTypeName + "Left.png", 0.1f, 2);
			}
			if (canMoveRight) {
				m_State = State::MovingRight;
				GetGameObject()->GetComponent<TextureComponent>()->SetTexture(m_EnemyTypeName + "Right.png", 0.1f, 2);
			}
		}


		//TODO: Calcualte chance of willing to move up or down when coming from a platform, or to continue
		if (canMoveDown && canMoveUp) {
			if (MathLib::CalculateChance() <= 0.5f) {
				m_State = State::MovingDown;
				GetGameObject()->GetComponent<TextureComponent>()->SetTexture(m_EnemyTypeName + "Down.png", 0.1f, 2);
			}
			else {
				m_State = State::MovingUp;
				GetGameObject()->GetComponent<TextureComponent>()->SetTexture(m_EnemyTypeName + "Up.png", 0.1f, 2);
			}
		}
		else {
			if (canMoveDown) {
				m_State = State::MovingDown;
				GetGameObject()->GetComponent<TextureComponent>()->SetTexture(m_EnemyTypeName + "Down.png", 0.1f, 2);
			}
			if (canMoveUp) {
				m_State = State::MovingUp;
				GetGameObject()->GetComponent<TextureComponent>()->SetTexture(m_EnemyTypeName + "Up.png", 0.1f, 2);
			}
		}
	}
}

void dae::EnemyComponent::CheckHit(GameObject* go)
{
	if (go->GetComponent<BurgerComponent>()->GetState() == BurgerState::FALLING || go->GetComponent<BurgerComponent>()->GetState() == BurgerState::FALLING_NO_INTERUPT) {
		auto rect{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
		rect.h /= 2;
		rect.y -= rect.h;
		auto rect2{ go->GetComponent<TextureComponent>()->GetRect() };
		if (MathLib::IsOverlapping(rect, rect2)) {
			if (m_State != State::Dying) {
				DestroyEnemy();
			}
		}
	}
}

void dae::EnemyComponent::Respawn()
{
	m_CanDie = false;
	m_DeathTimer = .4f;
	m_Lives = 1;
	m_CanChangeState = false;
	m_State = State::MovingRight;
	GetGameObject()->GetTransform()->Translate(m_StartPos);
	GetGameObject()->GetComponent<TextureComponent>()->SetTexture(m_EnemyTypeName + "Down.png", 0.1f, 2);
}

void dae::EnemyComponent::Stun()
{
	//TODO make enemies gray when stunned
	if (m_State != State::Stunned) {
		m_PrevState = m_State;
		m_State = State::Stunned;
	}
}

void dae::EnemyComponent::Render() const
{
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomLeft);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 0, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomRight);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftUp);
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftDown);
}

void dae::EnemyComponent::DestroyEnemy()
{
	auto go{ m_Scene->GetGameObject(EnumStrings[ScoreHolder]) };
	if (go) {
		go->GetComponent<ValuesComponent>()->IncreaseScore(m_Score);
	}
	m_State = State::Dying;
	GetGameObject()->GetComponent<TextureComponent>()->SetTexture(m_EnemyTypeName + "Dead.png", 0.1f, 2);
	m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayDeathSound(false);

	auto go2{ std::make_shared<GameObject>() };
	m_Scene->Add(go2);
	go2->AddComponent(std::make_unique<FloatingScoreComponent>(std::to_string(m_Score), GetGameObject()->GetTransform()->GetWorldPosition()));
	//GetGameObject()->MarkForDestroy();
}
