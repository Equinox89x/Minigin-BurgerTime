#include "EnemyComponent.h"
#include "TransformComponent.h"
#include <Timer.h>
#include <random>
#include <Minigin.h>
#include <glm/gtx/rotate_vector.hpp>
#include "Renderer.h"
#include "PlayerComponent.h"
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>
#include "AudioComponent.h"

void dae::EnemyComponent::Initialize()
{
	
}

void dae::EnemyComponent::Update()
{
	//float deltaTime{ Timer::GetInstance().GetDeltaTime() };
}

void dae::EnemyComponent::Render() const
{
	//SDL_Rect rect1{ static_cast<int>(EndPosition.x + 10), static_cast<int>(EndPosition.y + 10), 10,10 };
	//SDL_SetRenderDrawColor(Renderer::GetInstance().GetSDLRenderer(), 0, 0, 255, 255); // Set the color to blue
	//SDL_RenderDrawRect(Renderer::GetInstance().GetSDLRenderer(), &rect1); // D
}

void dae::EnemyComponent::DestroyEnemy()
{
	
}
