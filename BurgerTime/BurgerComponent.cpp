#include "BurgerComponent.h"
#include "Timer.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include <GalagaMath.h>


void dae::BurgerComponent::Update()
{
	float dt = Timer::GetInstance().GetDeltaTime();
	switch (m_BurgerState)
	{
	case BurgerState::FALLING_NO_INTERUPT:
		GetGameObject()->GetTransform()->AddTranslate(0, dt * m_FallSpeed);
		m_NoInterruptTimer -= dt;
		if (m_NoInterruptTimer <= 0) {
			m_NoInterruptTimer = m_NoInterruptTimerDefault;
			SetState(BurgerState::FALLING);
		}
		break;
	case BurgerState::FALLING:
		GetGameObject()->GetTransform()->AddTranslate(0, dt * m_FallSpeed);
		break;
	case BurgerState::STATIC:
		
		break;
	case BurgerState::FINISHED:

		break;
	default:
		break;
	}
}

void dae::BurgerComponent::HandleOverlap(GameObject* otherObj)
{
	if (m_BurgerState == BurgerState::FINISHED) return;

	auto rect{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
	auto otherRect{ otherObj->GetComponent<TextureComponent>()->GetRect() };
	switch (m_BurgerState)
	{
	case BurgerState::STATIC:
		if (GalagaMath::IsOverlapping(otherRect, rect)) {
			GetGameObject()->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING_NO_INTERUPT);

		}
		break;
	//case BurgerState::FALLING_NO_INTERUPT:
	//	if (GalagaMath::IsOverlapping(otherRect, rect)) {
	//		GetGameObject()->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING);
	//	}
	//	break;
	case BurgerState::FALLING:
		if (GalagaMath::IsOverlapping(otherRect, rect)) {
			GetGameObject()->GetComponent<BurgerComponent>()->SetState(otherObj->GetComponent<BurgerComponent>()->GetState() == BurgerState::FINISHED ? BurgerState::FINISHED : BurgerState::STATIC);
		}
		break;
	default:
		break;
	}
}

void dae::BurgerComponent::HandleOverlap(SDL_Rect& otherRect)
{
	if (m_BurgerState == BurgerState::FINISHED) return;

	auto rect{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
	switch (m_BurgerState)
	{
	case BurgerState::STATIC:
		if (GalagaMath::IsOverlapping(otherRect, rect)) {
			GetGameObject()->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING_NO_INTERUPT);

		}
		break;
	//case BurgerState::FALLING_NO_INTERUPT:
	//	if (GalagaMath::IsOverlapping(otherRect, rect)) {
	//		GetGameObject()->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING);
	//	}
	//	break;
	case BurgerState::FALLING:
		if (GalagaMath::IsOverlapping(otherRect, rect)) {
			GetGameObject()->GetComponent<BurgerComponent>()->SetState(BurgerState::STATIC);
		}
		break;
	default:
		break;
	}
}

void dae::BurgerComponent::HandlePlatformOverlap(SDL_Rect& otherRect, bool isFinished)
{
	if (m_BurgerState != BurgerState::FALLING) return;
	
	auto rect{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
	if (GalagaMath::IsOverlapping(otherRect, rect)) {
		GetGameObject()->GetComponent<BurgerComponent>()->SetState(isFinished ? BurgerState::FINISHED : BurgerState::STATIC);
	}
	
}
