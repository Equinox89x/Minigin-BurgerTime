#include "TransformComponent.h"
#include "GameObject.h"
#include "FloatingScoreComponent.h"
#include "TextObjectComponent.h"
#include "ResourceManager.h"
#include "Timer.h"

void dae::FloatingScoreComponent::Initialize()
{
	auto font = ResourceManager::GetInstance().LoadFont("Emulogic-zrEw.ttf", 18);
	GetGameObject()->AddComponent(std::make_unique<TextObjectComponent>(m_Text, font));
	GetGameObject()->GetTransform()->Translate(m_Pos);
}

void dae::FloatingScoreComponent::Update()
{
	float deltaTime{ Timer::GetInstance().GetDeltaTime() };
	m_DeathTimer-=deltaTime;
	if (m_DeathTimer < 0) {

		GetGameObject()->MarkForDestroy();
		return;
	}

	auto speed = m_Speed *= deltaTime;
	GetGameObject()->GetTransform()->AddTranslate(0, -speed);
}
