#pragma once
#include "Component.h"
#include "Subject.h"
#include <SDL_rect.h>
namespace dae {

	class PlatformComponent final : public Component
	{
	public:
		PlatformComponent(SDL_Rect rect);
		~PlatformComponent() = default;
		PlatformComponent(const PlatformComponent&) = delete;
		PlatformComponent(PlatformComponent&&) noexcept = delete;
		PlatformComponent& operator=(const PlatformComponent&) = delete;
		PlatformComponent& operator=(PlatformComponent&&) noexcept = delete;

		//void Initialize() override;
		void Update() override;
		void Render() const;

	private:
		SDL_Rect m_Rect{};
	};
}
