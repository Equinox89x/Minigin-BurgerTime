#include "PlayerComponent.h"
#include "GameObject.h"
#include <TextureComponent.h>
#include <ValuesComponent.h>
#include <Timer.h>
#include <Renderer.h>
#include "AudioComponent.h"
#include <MoveKeyboardComponent.h>
#include "Observers.h"
#include <MoveControllerComponent.h>
#include "EnemyComponent.h"

void dae::PlayerComponent::Update()
{
	auto deltaTime{ Timer::GetInstance().GetDeltaTime() };
	switch (m_PlayerState)
	{
	case dae::PlayerComponent::ALIVE:
		if (!m_IsOpposer) {
			HandleEnemyOverlap();
		}
		else {
			HandlePlayerOverlap();
		}
		break;
	case dae::PlayerComponent::DEAD:
		m_DeathTimer -= deltaTime;
		if (m_DeathTimer < m_DefaultDeathTimer - 1.5f) {
			HandleDeathEnd();
		}
		break;
	case dae::PlayerComponent::RESPAWN:
		m_DeathTimer -= deltaTime;
		if (m_DeathTimer <= 0) {
			HandleRespawn();
		}
		break;
	case PlayerState::THROW:
		m_SaltThrowTimer -= deltaTime;
		if (m_SaltThrowTimer <= 0) {
			m_SaltThrowTimer = m_DefaultSaltThrowTimer;
			m_PlayerState = PlayerState::ALIVE;
			std::string deathName;
			if (m_IsOpposer) {
				deathName = "hotdogDown.png";
			}
			else {
				deathName = GetGameObject()->GetName() == EnumStrings[Player0] ? "moveDown.png" : "moveDownSalt.png";
			}
			auto rect = GetGameObject()->GetComponent<TextureComponent>()->GetRect();
			GetGameObject()->GetTransform()->Translate(m_PrevLoc);
			GetGameObject()->GetComponent<TextureComponent>()->SetTexture(deathName, 0.3f, 3);
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

void dae::PlayerComponent::CheckMovement(const std::vector<std::pair<SDL_Rect, GameObject*>>& rects, MovementDirection moveDirection)
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

	bool canMoveRight{ false };
	bool canMoveLeft{ false };
	bool isOverlappingLeft{ false };

	for (auto item : rects)
	{
		switch (moveDirection)
		{
		case dae::PlayerComponent::VERTICAL:
			if (MathLib::IsOverlapping(m_LeftRect, item.first)) {
				isOverlappingLeft = true;
				break;
			}
			break;
		case dae::PlayerComponent::HORIZONTAL:
				if (MathLib::IsOverlapping(m_BottomRight, item.first)) { 
					canMoveRight = true;
				}
				if (MathLib::IsOverlapping(m_BottomLeft, item.first)) { 
					canMoveLeft = true;
				}
			break;
		default:
			break;
		}
	}

	switch (moveDirection)
	{
	case dae::PlayerComponent::VERTICAL:
		if (!m_IsController) {
			GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(MathLib::Side::Top, isOverlappingLeft);
			GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(MathLib::Side::Bottom, isOverlappingLeft);
		}
		else{
			GetGameObject()->GetComponent<MoveControllerComponent>()->SetCanMove(MathLib::Side::Top, isOverlappingLeft);
			GetGameObject()->GetComponent<MoveControllerComponent>()->SetCanMove(MathLib::Side::Bottom, isOverlappingLeft);
		}
		break;
	case dae::PlayerComponent::HORIZONTAL:
		if (!m_IsController) {
			GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(MathLib::Side::Left, canMoveLeft);
			GetGameObject()->GetComponent<MoveKeyboardComponent>()->SetCanMove(MathLib::Side::Right, canMoveRight);		
		}
		else{
			GetGameObject()->GetComponent<MoveControllerComponent>()->SetCanMove(MathLib::Side::Left, canMoveLeft);
			GetGameObject()->GetComponent<MoveControllerComponent>()->SetCanMove(MathLib::Side::Right, canMoveRight);
		}
		break;
	default:
		break;
	}
}

void dae::PlayerComponent::HandlePlayerOverlap() {
	if (auto player{ m_Scene->GetGameObject(EnumStrings[Player0]) }) {
		if (!player->IsMarkedForDestroy()) {
			auto rect1{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
			auto rect2{ player->GetComponent<TextureComponent>()->GetRect() };
			if (MathLib::IsOverlapping(rect1, rect2)) {
				player->GetComponent<PlayerComponent>()->Die();
			}
		}
	}
}
void dae::PlayerComponent::ThrowSalt()
{
	auto values{m_Scene->GetGameObject(EnumStrings[ScoreHolder])->GetComponent<ValuesComponent>()};
	if (m_PlayerState != PlayerState::THROW && m_Movement != Movement::UP && m_Movement != Movement::DOWN && values->GetSalt()) {
		values->DecreaseSalt();
		auto comp{ GetGameObject()->GetComponent<TextureComponent>() };
		auto rect = GetGameObject()->GetComponent<TextureComponent>()->GetRect();
		float distance{ 0 };
		switch (m_Movement)
		{
		case dae::PlayerComponent::LEFT:
			comp->SetTexture("SaltThrowLeft.png", 0.3f, 3);
			distance = float(-rect.w*6);
			break;
		case dae::PlayerComponent::RIGHT:
			comp->SetTexture("SaltThrow.png", 0.3f, 3);
			distance = float(-rect.w );
			break;
		default:
			break;
		}


		m_PrevLoc = GetGameObject()->GetTransform()->GetWorldPosition();
		GetGameObject()->GetTransform()->AddTranslate(distance, 0);
		m_PlayerState = PlayerState::THROW;


		if (auto go{ m_Scene->GetGameObject(EnumStrings[EnemyHolder]) }) {
			auto children{ go->GetChildren(EnumStrings[Enemy]) };
			auto rect1{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
			for (auto enemy : children) {
				auto rect2{ enemy->GetComponent<TextureComponent>()->GetRect() };
				if (MathLib::IsOverlapping(rect1, rect2)) {
					enemy->GetComponent<EnemyComponent>()->Stun();
				}
			}
		}
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
				if (m_PlayerState == PlayerState::THROW) {
					enemy->GetComponent<EnemyComponent>()->Stun();
				}
				else {
					Die();
				}
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

	std::string deathName;
	if (m_IsOpposer) {
		deathName = "hotdogDown.png";
	}
	else {
		deathName = player->GetName() == EnumStrings[Player0] ? "moveDown.png" : "moveDownSalt.png";
	}
	player->GetComponent<TextureComponent>()->SetTexture(deathName, 0.3f, 3);

	m_PlayerState = PlayerState::RESPAWN;
}

void dae::PlayerComponent::HandleRespawn()
{
	m_DeathTimer = m_DefaultDeathTimer;
	auto player{ GetGameObject() };
	player->GetComponent<TextureComponent>()->SetIsVisible(true);
	Reposition();
	GetGameObject()->EnableCollision(true);
	CanBeGrabbed = true;
	m_PlayerState = PlayerState::ALIVE;
}

void dae::PlayerComponent::Die()
{
	m_PlayerState = PlayerState::DEAD;
	auto player{ GetGameObject() };
	player->GetComponent<TextureComponent>()->Scale(3, 3);

	std::string deathName;
	if (m_IsOpposer) {
		deathName = "hotdogDead.png";
		m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayDeathSound(false);
	}
	else {
		deathName = player->GetName() == EnumStrings[Player0] ? "playerDeath.png" : "playerDeathSalt.png";
		m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayPlayerDeathSound(false);
	}
	player->GetComponent<TextureComponent>()->SetTexture(deathName, 0.3f, 5);
	auto values{ m_Scene->GetGameObject(EnumStrings[ScoreHolder])->GetComponent<ValuesComponent>() };
	values->Damage();

	player->EnableCollision(false);

}