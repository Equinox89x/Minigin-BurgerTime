#include "PlayerComponent.h"
#include "GameObject.h"
#include <TextureComponent.h>
#include <ValuesComponent.h>
#include <Timer.h>
#include <Renderer.h>
#include "AudioComponent.h"
#include <MoveKeyboardComponent.h>

void dae::PlayerComponent::Update()
{
	auto deltaTime{ Timer::GetInstance().GetDeltaTime() };
	switch (m_PlayerState)
	{
	case dae::PlayerComponent::ALIVE:
		HandleEnemyOverlap();
		break;
	case dae::PlayerComponent::DEAD:
		DeathTimer -= deltaTime;
		if (DeathTimer < DefaultDeathTimer - 0.3f) {
			HandleDeathEnd();
		}
		break;
	case dae::PlayerComponent::RESPAWN:
		DeathTimer -= deltaTime;
		if (DeathTimer <= 0) {
			HandleRespawn();
		}
		break;
	default:
		break;
	}
}

void dae::PlayerComponent::Render() const
{
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomLeft);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 0, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_BottomRight);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 255, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_LeftRect);
}

void dae::PlayerComponent::CheckMovement(const std::vector<std::pair<SDL_Rect, GameObject*>>& rects, bool isVertical)
{
	m_Rect = GetGameObject()->GetComponent<TextureComponent>()->GetRect();

	const auto xLeft{ m_Rect.x };
	const auto xRight{ m_Rect.x + m_Rect.w };
	m_BottomLeft = SDL_Rect{ xLeft, m_Rect.y + m_Rect.h, m_Rect.w / 3, m_Rect.h / 3 };
	m_BottomRight = SDL_Rect{ xRight, m_Rect.y + m_Rect.h, m_Rect.w / 3, m_Rect.h / 3 };
	m_BottomRight.x -= m_BottomRight.w;

	m_LeftRect = m_Rect;
	m_LeftRect.w /= 5;
	m_LeftRect.x -= m_LeftRect.w;
	m_LeftRect.h /= 2;
	m_LeftRect.y += m_Rect.h / 4;

	//m_BottomRect = m_Rect;
	//m_BottomRect.h /= 5;
	//m_BottomRect.y += m_Rect.h;

	bool canMoveRight{ false };
	bool canMoveLeft{ false };
	bool isOverlappingLeft{ false };

	for (auto item : rects)
	{
		if (!isVertical) {
			//if (MathLib::IsOverlapping(m_Rect, item.first)) {
				if (MathLib::IsOverlapping(m_BottomRight, item.first)) { 
					canMoveRight = true;
				}
				if (MathLib::IsOverlapping(m_BottomLeft, item.first)) { 
					canMoveLeft = true;
				}
				//break;
			//}
		}
		else {
			if (MathLib::IsOverlapping(m_LeftRect, item.first)) {
				isOverlappingLeft = true;
				break;
			}
		}
	}
	if (!isVertical) {
		GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(MathLib::Side::Left, canMoveLeft);
		GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(MathLib::Side::Right, canMoveRight);
	}
	else {
		GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(MathLib::Side::Top, isOverlappingLeft);
		GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(MathLib::Side::Bottom, isOverlappingLeft);
	}
}

void dae::PlayerComponent::HandleEnemyOverlap()
{
	if(auto go{ m_Scene->GetGameObject(EnumStrings[EnemyHolder]) }) {
		auto children{ go->GetChildren(EnumStrings[Enemy]) };
		auto rect1{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
		for (auto enemy : children) {
			if (enemy->IsMarkedForDestroy()) continue;
			auto rect2{ enemy->GetComponent<TextureComponent>()->GetRect() };
			if (MathLib::IsOverlapping(rect1, rect2)) {
				Die();
				enemy->MarkForDestroy();
				return;
			}
		}
	}
}

void dae::PlayerComponent::HandleDeathEnd()
{
	auto player{ GetGameObject() };
	player->GetComponent<TextureComponent>()->SetIsVisible(false);
	player->GetComponent<TextureComponent>()->Scale(3,3);
	player->GetComponent<TextureComponent>()->SetTexture(player->GetName() == EnumStrings[Player0] ? "moveUp.png" : "moveUp.png", 0.1f, 3);
	auto rect = player->GetComponent<TextureComponent>()->GetRect();
	player->GetComponent<TextureComponent>()->SetOffset({ (rect.w * 1.5f) - 1.5f, 0.f });

	m_PlayerState = PlayerState::RESPAWN;
}

void dae::PlayerComponent::HandleRespawn()
{
	DeathTimer = DefaultDeathTimer;
	auto player{ GetGameObject() };
	player->GetComponent<TextureComponent>()->SetIsVisible(true);
	player->GetComponent<TextureComponent>()->SetPosition((GameWindowSizeX / 2) - (Margin * 2), WindowSizeY - ((Margin * 3) + WindowBuffer));
	GetGameObject()->EnableCollision(true);
	CanBeGrabbed = true;
	m_PlayerState = PlayerState::ALIVE;
}

void dae::PlayerComponent::Die()
{
	m_PlayerState = PlayerState::DEAD;
	auto player{ GetGameObject() };
	player->GetComponent<TextureComponent>()->Scale(2.5f, 2.5f);
	player->GetComponent<TextureComponent>()->SetTexture("playerDeath.png", 0.1f, 5);
	auto rect = player->GetComponent<TextureComponent>()->GetRect();
	player->GetComponent<TextureComponent>()->SetOffset({ -rect.w / 2, -rect.h / 2 });
	auto values{ m_Scene->GetGameObject(EnumStrings[ScoreHolder])->GetComponent<ValuesComponent>() };
	values->Damage();

	player->EnableCollision(false);

	m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayPlayerDeathSound(false);
}