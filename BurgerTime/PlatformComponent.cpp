#include "PlatformComponent.h"
#include "Renderer.h"

dae::PlatformComponent::PlatformComponent(SDL_Rect rect) : 
	m_Rect{ rect }
{}

void dae::PlatformComponent::Update()
{
}

void dae::PlatformComponent::Render() const
{
	SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 255, 0, 0, 255); // Set the color to red
	SDL_RenderFillRect(Renderer::GetInstance().GetSDLRenderer(), &m_Rect);
}
