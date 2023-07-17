#include "BurgerComponent.h"
#include "Timer.h"
#include "TransformComponent.h"

void dae::BurgerComponent::Update()
{
	float dt = Timer::GetInstance().GetDeltaTime();
	switch (m_BurgerState)
	{
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
