#include "BurgerManager.h"
#include "TextureComponent.h"
#include "BurgerComponent.h"
#include "Renderer.h"
#include "PlayerComponent.h"

void dae::BurgerManager::Update()
{
	auto player{ m_Scene->GetGameObject(EnumStrings[Player0]) };
	SDL_Rect charRect{ player->GetComponent<TextureComponent>()->GetRect() };
	for (std::map<std::string, GameObject*>& map : m_Burgers) {
		auto pattyTop{ map[EnumStrings[PattyTop]] };
		auto pattyBottom{ map[EnumStrings[PattyBottom]] };
		auto veggie{ map[EnumStrings[Veggie]] };
		auto burger{ map[EnumStrings[Burger]] };

		//player overlap
		pattyTop->GetComponent<BurgerComponent>()->HandleOverlap(charRect);
		pattyBottom->GetComponent<BurgerComponent>()->HandleOverlap(charRect);
		veggie->GetComponent<BurgerComponent>()->HandleOverlap(charRect);
		burger->GetComponent<BurgerComponent>()->HandleOverlap(charRect);

		//burger overlap
		pattyTop->GetComponent<BurgerComponent>()->HandleOverlap(veggie);
		veggie->GetComponent<BurgerComponent>()->HandleOverlap(burger);
		burger->GetComponent<BurgerComponent>()->HandleOverlap(pattyBottom);

		//player movement overlap
		player->GetComponent<PlayerComponent>()->CheckMovement(m_Platforms, false);
		player->GetComponent<PlayerComponent>()->CheckMovement(m_pLadders, true);

		//platform overlap
		for (const auto& platform : m_Platforms) {
			auto rect{ platform.first };
			pattyTop->GetComponent<BurgerComponent>()->HandlePlatformOverlap(rect);
			pattyBottom->GetComponent<BurgerComponent>()->HandlePlatformOverlap(rect);
			veggie->GetComponent<BurgerComponent>()->HandlePlatformOverlap(rect);
			burger->GetComponent<BurgerComponent>()->HandlePlatformOverlap(rect);

		}

		//for (const auto& ladder : m_pLadders)
		//{
		//	auto rect{ ladder.first };
		//}

		for (const auto& plate : m_pPlates) {
			auto rect{ plate.first };
			pattyBottom->GetComponent<BurgerComponent>()->HandlePlatformOverlap(rect, true);
		}
	}
}

void dae::BurgerManager::Render() const
{
	//for (const auto& platform : m_Platforms) {
	//	auto rect{ platform.first };
	//	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
	//	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
	//}	
	//for (const auto& plate : m_pPlates) {
	//	auto rect{ plate.first };
	//	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
	//	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
	//}	
	//for (const auto& ladder : m_pLadders) {
	//	auto rect{ ladder.first };
	//	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
	//	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect);
	//}
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
