#include "BurgerManager.h"
#include "TextureComponent.h"
#include "BurgerComponent.h"
#include "Renderer.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"

void dae::BurgerManager::Update()
{
	auto enemyHolder{ m_Scene->GetGameObject(EnumStrings[EnemyHolder]) };
	auto player{ m_Scene->GetGameObject(EnumStrings[Player0]) };
	SDL_Rect charRect{ player->GetComponent<TextureComponent>()->GetRect() };
	for (std::map<std::string, GameObject*>& map : m_Burgers) {
		auto pattyTop{ map[EnumStrings[PattyTop]] };
		auto pattyBottom{ map[EnumStrings[PattyBottom]] };
		auto veggie{ map[EnumStrings[Veggie]] };
		auto burger{ map[EnumStrings[Burger]] };

		auto pattyTopComponent{ pattyTop->GetComponent<BurgerComponent>()};
		auto pattyBottomComponent{pattyBottom->GetComponent<BurgerComponent>()};
		auto veggieComponent{veggie->GetComponent<BurgerComponent>()};
		auto burgerComponent{ burger->GetComponent<BurgerComponent>() };

		//platform overlap
		for (const auto& platform : m_Platforms) {
			auto rect{ platform.first };
			pattyTopComponent->HandlePlatformOverlap(rect);
			pattyBottomComponent->HandlePlatformOverlap(rect);
			veggieComponent->HandlePlatformOverlap(rect);
			burgerComponent->HandlePlatformOverlap(rect);
		}

		for (const auto& plate : m_pPlates) {
			auto rect{ plate.first };
			pattyBottom->GetComponent<BurgerComponent>()->HandlePlatformOverlap(rect, true);
		}

		//player overlap
		pattyTopComponent->HandleOverlap(charRect);
		pattyBottomComponent->HandleOverlap(charRect);
		veggieComponent->HandleOverlap(charRect);
		burgerComponent->HandleOverlap(charRect);

		//burger overlap
		pattyTopComponent->HandleOverlap(veggie);
		veggieComponent->HandleOverlap(burger);
		burgerComponent->HandleOverlap(pattyBottom);

		//Enemy movement overlap
		for (auto enemy : enemyHolder->GetChildren()) {
			enemy->GetComponent<EnemyComponent>()->CheckHit(pattyTop);
			enemy->GetComponent<EnemyComponent>()->CheckHit(pattyBottom);
			enemy->GetComponent<EnemyComponent>()->CheckHit(veggie);
			enemy->GetComponent<EnemyComponent>()->CheckHit(burger);
		}
	}
	//player movement overlap
	player->GetComponent<PlayerComponent>()->CheckMovement(m_Platforms, false);
	player->GetComponent<PlayerComponent>()->CheckMovement(m_pLadders, true);

	//Enemy movement overlap
	for (auto enemy : enemyHolder->GetChildren()) {
		enemy->GetComponent<EnemyComponent>()->CheckMovement(m_Platforms, m_pLadders);
	}
}

void dae::BurgerManager::Render() const
{
	for (const auto& platform : m_Platforms) {
		auto rect{ platform.first };
		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
	}	
	for (const auto& plate : m_pPlates) {
		auto rect{ plate.first };
		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
	}	
	for (const auto& ladder : m_pLadders) {
		auto rect{ ladder.first };
		SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
		SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
	}
}

bool dae::BurgerManager::GetAreBurgersFinished()
{
	for (auto burger : m_Burgers)
	{
		for (auto burgerPart : burger)
		{
			if (burgerPart.second->GetComponent<BurgerComponent>()->GetState() != BurgerState::FINISHED) {
				return false;
			}
		}
	}
	return true;
}

std::map<std::string, dae::GameObject*> dae::BurgerManager::AddBurger(GameObject* pattyTop, GameObject* pattyBottom, GameObject* veggie, GameObject* burger)
{
	std::map<std::string, GameObject*> map{
		std::make_pair(EnumStrings[PattyTop], pattyTop),
		std::make_pair(EnumStrings[PattyBottom], pattyBottom),
		std::make_pair(EnumStrings[Veggie], veggie),
		std::make_pair(EnumStrings[Burger], burger),
	};
	m_Burgers.push_back(map);
	return map;
}

std::pair<SDL_Rect, dae::GameObject*> dae::BurgerManager::AddPlatform(SDL_Rect rect, GameObject* go)
{
	auto pair{ std::make_pair(rect, go) };
	m_Platforms.push_back(pair);
	return pair;
}

std::pair<SDL_Rect, dae::GameObject*> dae::BurgerManager::AddLadder(SDL_Rect rect, GameObject* go)
{
	auto pair{ std::make_pair(rect, go) };
	m_pLadders.push_back(pair);
	return pair;
}

std::pair<SDL_Rect, dae::GameObject*> dae::BurgerManager::AddPlate(SDL_Rect rect, GameObject* go)
{
	auto pair{ std::make_pair(rect, go) };
	m_pPlates.push_back(pair);
	return pair;
}
