#include "EndScreenComponent.h"
#include "Timer.h"
#include "Subject.h"

void dae::EndScreenComponent::Update()
{
	float dt{ Timer::GetInstance().GetDeltaTime() };
	EndTimer -= dt;
	if (EndTimer <= 0) {
		Event menu{ EventType::ToMenu };
		Notify(GetGameObject(), menu);
	}
}
