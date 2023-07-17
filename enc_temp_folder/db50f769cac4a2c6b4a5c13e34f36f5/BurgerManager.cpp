#include "BurgerManager.h"
#include "TextureComponent.h"
#include <GalagaMath.h>
#include <unordered_map>
#include "BurgerComponent.h"

void dae::BurgerManager::Update()
{
	auto player{ m_Scene->GetGameObject(EnumStrings[Player0]) };
	SDL_Rect charRect{ player->GetComponent<TextureComponent>()->GetRect() };
	for (std::map<std::string, GameObject*>& map : m_Burgers) {
		auto pattyTop{ map[EnumStrings[PattyTop]] };
		auto pattyBottom{ map[EnumStrings[PattyBottom]] };
		auto veggie{ map[EnumStrings[Veggie]] };
		auto burger{ map[EnumStrings[Burger]] };

		auto pattyTopRect{ pattyTop->GetComponent<TextureComponent>()->GetRect() };
		auto pattyBottomRect{ pattyBottom->GetComponent<TextureComponent>()->GetRect() };
		auto veggieRect{ veggie->GetComponent<TextureComponent>()->GetRect() };
		auto burgerRect{ burger->GetComponent<TextureComponent>()->GetRect() };

		//player overlap
		if (GalagaMath::IsOverlapping(charRect, pattyTopRect)) {
			pattyTop->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING);
		}		
		if (GalagaMath::IsOverlapping(charRect, pattyBottomRect)) {
			pattyBottom->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING);
		}
		if (GalagaMath::IsOverlapping(charRect, veggieRect)) {
			veggie->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING);
		}
		if (GalagaMath::IsOverlapping(charRect, burgerRect)) {
			burger->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING);
		}

		//burger overlap
		if (GalagaMath::IsOverlapping(pattyTopRect, veggieRect)) {
			veggie->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING);
		}
		if (GalagaMath::IsOverlapping(veggieRect, burgerRect)) {
			burger->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING);
		}
		if (GalagaMath::IsOverlapping(burgerRect, pattyBottomRect)) {
			pattyBottom->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING);
		}
	}
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
