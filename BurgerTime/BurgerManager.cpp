#include "BurgerManager.h"
#include "TextureComponent.h"
#include "BurgerComponent.h"
#include "Renderer.h"
#include "PlayerComponent.h"
#include "EnemyComponent.h"

void dae::BurgerManagerComponent::Update()
{
	const auto enemyHolder{ m_Scene->GetGameObject(EnumStrings[EnemyHolder]) };
	const auto player{ m_Scene->GetGameObject(EnumStrings[Player0]) };
	SDL_Rect charRect{ player->GetComponent<TextureComponent>()->GetRect() };
	if (m_Burgers.size() > 0) {
		for (std::map<std::string, GameObject*>& map : m_Burgers) {
			HandleBurgerOverlap(map, enemyHolder, charRect);
		}
	}

	//player movement overlap
	if (m_Platforms.size() > 0)
		player->GetComponent<PlayerComponent>()->CheckMovement(m_Platforms, false);
	if (m_pLadders.size() > 0)
	player->GetComponent<PlayerComponent>()->CheckMovement(m_pLadders, true);

	//Enemy movement overlap
	if (enemyHolder && enemyHolder->GetChildren().size() > 0) {
		for (auto enemy : enemyHolder->GetChildren()) {
			enemy->GetComponent<EnemyComponent>()->CheckMovement(m_Platforms, m_pLadders);
		}
	}
}

void dae::BurgerManagerComponent::HandleBurgerOverlap(std::map<std::string, GameObject*>& map, const std::shared_ptr<GameObject>& enemyHolder, SDL_Rect& charRect)
{
	
	auto pattyTop{ map[EnumStrings[PattyTop]] };
	auto pattyBottom{ map[EnumStrings[PattyBottom]] };
	auto veggie{ map[EnumStrings[Veggie]] };
	auto burger{ map[EnumStrings[Burger]] };

	auto pattyTopComponent{ pattyTop->GetComponent<BurgerComponent>() };
	auto pattyBottomComponent{ pattyBottom->GetComponent<BurgerComponent>() };
	auto veggieComponent{ veggie->GetComponent<BurgerComponent>() };
	auto burgerComponent{ burger->GetComponent<BurgerComponent>() };

	auto pattyTopRect{ pattyTop->GetComponent<TextureComponent>()->GetRect() };
	auto pattyBottomRect{ pattyBottom->GetComponent<TextureComponent>()->GetRect() };
	auto veggieRect{ veggie->GetComponent<TextureComponent>()->GetRect() };
	auto burgerRect{ burger->GetComponent<TextureComponent>()->GetRect() };

	if (MathLib::IsOverlapping(pattyTopRect, veggieRect)) {
		if (veggieComponent->GetState() != BurgerState::FINISHED) {
			veggieComponent->SetState(BurgerState::FALLING_NO_INTERUPT);
		}
		else {
			pattyTopComponent->SetState(BurgerState::FINISHED);
			CheckAreBurgersFinished();
		}

	}
	if (MathLib::IsOverlapping(veggieRect, burgerRect)) {
		if (burgerComponent->GetState() != BurgerState::FINISHED) {
			burgerComponent->SetState(BurgerState::FALLING_NO_INTERUPT);
		}
		else {
			veggieComponent->SetState(BurgerState::FINISHED);
			CheckAreBurgersFinished();
		}


	}
	if (MathLib::IsOverlapping(burgerRect, pattyBottomRect)) {
		if (pattyBottomComponent->GetState() != BurgerState::FINISHED) {
			pattyBottomComponent->SetState(BurgerState::FALLING_NO_INTERUPT);
		}
		else {
			burgerComponent->SetState(BurgerState::FINISHED);
			CheckAreBurgersFinished();
		}

	}

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

	//Enemy movement overlap
	for (auto enemy : enemyHolder->GetChildren()) {
		enemy->GetComponent<EnemyComponent>()->CheckHit(pattyTop);
		enemy->GetComponent<EnemyComponent>()->CheckHit(pattyBottom);
		enemy->GetComponent<EnemyComponent>()->CheckHit(veggie);
		enemy->GetComponent<EnemyComponent>()->CheckHit(burger);
	}
}

void dae::BurgerManagerComponent::Render() const
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

bool dae::BurgerManagerComponent::CheckAreBurgersFinished()
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
	Event stageCleared{ EventType::StageCleared };
	Notify(GetGameObject(), stageCleared);
	return true;
}

std::map<std::string, dae::GameObject*> dae::BurgerManagerComponent::AddBurger(GameObject* pattyTop, GameObject* pattyBottom, GameObject* veggie, GameObject* burger)
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

std::pair<SDL_Rect, dae::GameObject*> dae::BurgerManagerComponent::AddPlatform(SDL_Rect rect, GameObject* go)
{
	auto pair{ std::make_pair(rect, go) };
	m_Platforms.push_back(pair);
	return pair;
}

std::pair<SDL_Rect, dae::GameObject*> dae::BurgerManagerComponent::AddLadder(SDL_Rect rect, GameObject* go)
{
	auto pair{ std::make_pair(rect, go) };
	m_pLadders.push_back(pair);
	return pair;
}

std::pair<SDL_Rect, dae::GameObject*> dae::BurgerManagerComponent::AddPlate(SDL_Rect rect, GameObject* go)
{
	auto pair{ std::make_pair(rect, go) };
	m_pPlates.push_back(pair);
	return pair;
}
