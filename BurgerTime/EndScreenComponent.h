#pragma once
#include "Component.h"
#include "Subject.h"
namespace dae {

	class EndScreenComponent final : public Component, public Subject
	{
	public:
		EndScreenComponent() = default;
		~EndScreenComponent() = default;
		EndScreenComponent(const EndScreenComponent&) = delete;
		EndScreenComponent(EndScreenComponent&&) noexcept = delete;
		EndScreenComponent& operator=(const EndScreenComponent&) = delete;
		EndScreenComponent& operator=(EndScreenComponent&&) noexcept = delete;

		//void Initialize() override;
		void Update() override;

	private:
		float EndTimer{ 5 };

	};
}
