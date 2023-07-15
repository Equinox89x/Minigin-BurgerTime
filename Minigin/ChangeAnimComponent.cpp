#include "ChangeAnimComponent.h"
#include <TextureComponent.h>


void dae::ChangeAnimComponent::SetTexture(const PlayerComponent::Movement movement)
{
	switch (movement)
	{
	case PlayerComponent::Movement::UP:
		m_Player->GetComponent<TextureComponent>()->SetTexture("'moveUp.png");
		break;
	case PlayerComponent::Movement::DOWN:
		m_Player->GetComponent<TextureComponent>()->SetTexture("'moveDown.png");
		break;
	case PlayerComponent::Movement::LEFT:
		m_Player->GetComponent<TextureComponent>()->SetTexture("'moveLeft.png");
		break;
	case PlayerComponent::Movement::RIGHT:
		m_Player->GetComponent<TextureComponent>()->SetTexture("'moveRight.png");
		break;
	default:
		break;
	}
}
