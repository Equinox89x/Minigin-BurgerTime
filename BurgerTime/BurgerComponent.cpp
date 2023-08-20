#include "BurgerComponent.h"
#include "Timer.h"
#include "TextureComponent.h"
#include "TransformComponent.h"
#include <MathLib.h>
#include <ValuesComponent.h>
#include "AudioComponent.h"
#include "Renderer.h"


void dae::BurgerComponent::Initialize()
{
	UpdateRects();
}

void dae::BurgerComponent::UpdateRects()
{
	auto rect{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
	rect.h -= 5;
	rect.w /= 4;
	auto rect1{ SDL_Rect{ rect.x, rect.y, rect.w, rect.h } };
	auto rect2{ SDL_Rect{ rect.x + rect.w, rect.y, rect.w, rect.h } };
	auto rect3{ SDL_Rect{ rect.x + (rect.w * 2), rect.y, rect.w, rect.h } };
	auto rect4{ SDL_Rect{ rect.x + (rect.w * 3), rect.y, rect.w, rect.h } };
	m_Rects = { std::make_pair(rect1, false), std::make_pair(rect2, false), std::make_pair(rect3,false), std::make_pair(rect4, false) };
}

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

void dae::BurgerComponent::Render() const
{
	//auto rect{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
	//auto rect1{ SDL_Rect{rect.x, rect.y, rect.w / 4, rect.h} };
	//auto rect2{ SDL_Rect{rect.x + rect.w / 4, rect.y, rect.w / 4, rect.h} };
	//auto rect3{ SDL_Rect{rect.x + ((rect.w / 4)*2), rect.y, rect.w / 4, rect.h} };
	//auto rect4{ SDL_Rect{rect.x + ((rect.w / 4)*3), rect.y, rect.w / 4, rect.h} };
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 255, 0, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect1);	
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 255, 0, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect2);
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect3);	
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 255); // Set the color to red
	//SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &rect4);
}

void dae::BurgerComponent::HandleOverlap(GameObject* otherObj)
{
	if (m_BurgerState == BurgerState::FINISHED) return;

	auto rect{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
	auto otherRect{ otherObj->GetComponent<TextureComponent>()->GetRect() };

	switch (m_BurgerState)
	{
	case BurgerState::STATIC:
		if (MathLib::IsOverlapping(otherRect, rect)) {
			GetGameObject()->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING_NO_INTERUPT);
			m_Scene->GetGameObject(EnumStrings[ScoreHolder])->GetComponent<ValuesComponent>()->IncreaseScore(50);
		}
		break;
	case BurgerState::FALLING:
		if (MathLib::IsOverlapping(otherRect, rect)) {
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

	auto rectObj{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
	for (auto& rect : m_Rects) {
		if (!rect.second) {
			if (MathLib::IsOverlapping(otherRect, rect.first)) {
				rect.second = true;
				m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayShootSound(false);
			}
		}
	}
	switch (m_BurgerState)
	{
	case BurgerState::STATIC:
		if (!(std::any_of(m_Rects.begin(), m_Rects.end(), [](std::pair<SDL_Rect, bool> obj) { return !obj.second; }))) {
			GetGameObject()->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING_NO_INTERUPT);
			m_Scene->GetGameObject(EnumStrings[ScoreHolder])->GetComponent<ValuesComponent>()->IncreaseScore(50);
		}

		//if (MathLib::IsOverlapping(otherRect, rect)) {
		//	GetGameObject()->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING_NO_INTERUPT);
		//	m_Scene->GetGameObject(EnumStrings[ScoreHolder])->GetComponent<ValuesComponent>()->IncreaseScore(50);
		//}
		break;
	case BurgerState::FALLING:
		if (MathLib::IsOverlapping(otherRect, rectObj)) {
			GetGameObject()->GetComponent<BurgerComponent>()->SetState(BurgerState::STATIC);
			UpdateRects();
		}
		break;
	default:
		break;
	}

	/*auto rect{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
	switch (m_BurgerState)
	{
	case BurgerState::STATIC:
		if (MathLib::IsOverlapping(otherRect, rect)) {
			m_Scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayShootSound(false);
			GetGameObject()->GetComponent<BurgerComponent>()->SetState(BurgerState::FALLING_NO_INTERUPT);
			m_Scene->GetGameObject(EnumStrings[ScoreHolder])->GetComponent<ValuesComponent>()->IncreaseScore(50);
		}
		break;
	case BurgerState::FALLING:
		if (MathLib::IsOverlapping(otherRect, rect)) {
			GetGameObject()->GetComponent<BurgerComponent>()->SetState(BurgerState::STATIC);
		}
		break;
	default:
		break;
	}*/
}

void dae::BurgerComponent::HandlePlatformOverlap(SDL_Rect& otherRect, bool isFinished)
{
	if (m_BurgerState != BurgerState::FALLING) return;
	
	auto rect{ GetGameObject()->GetComponent<TextureComponent>()->GetRect() };
	if (MathLib::IsOverlapping(otherRect, rect)) {
		GetGameObject()->GetComponent<BurgerComponent>()->SetState(isFinished ? BurgerState::FINISHED : BurgerState::STATIC);
		UpdateRects();
	}
	
}
