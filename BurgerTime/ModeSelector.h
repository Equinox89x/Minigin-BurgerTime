#pragma once
#include "Component.h"
#include "Scene.h"
#include <functional>
#include "Minigin.h"
#include <glm/ext/vector_float2.hpp>


namespace dae
{
	class ModeSelector final : public Component
	{

	public:
		ModeSelector(dae::Scene* scene, std::function<void(dae::Scene*, glm::vec2)> createMainPlayer, std::function<void(dae::Scene*, glm::vec2 startPos)> createSecondaryPlayer, std::function<void(dae::Scene*)> createStage, std::function<void(dae::Scene*, glm::vec2 startPos)> createVersusStage, std::function<void(dae::Scene*)> createScore) : m_pScene{ scene } {
			CreateMainPlayer = createMainPlayer;
			CreateSecondaryPlayer = createSecondaryPlayer;
			CreateVersusStage = createVersusStage;
			CreateStage = createStage;
			CreateScore = createScore;

		};
		~ModeSelector() = default;
		ModeSelector(const ModeSelector&) = delete;
		ModeSelector(ModeSelector&&) noexcept = delete;
		ModeSelector& operator=(const ModeSelector&) = delete;
		ModeSelector& operator=(ModeSelector&&) noexcept = delete;

		void CycleGameMode(bool isMoveUp);
		void StartGame(GameObject* pMenu);

	private:
		enum class GameMode {
			SOLO, COOP, VERSUS
		};

		GameMode m_SelectedGameMode{ GameMode::SOLO };
		bool m_IsMainMenu{ true };
		bool m_IsMenu{ false };

		dae::Scene* m_pScene;

		std::function<void(dae::Scene* scene, glm::vec2 startPos)> CreateMainPlayer;
		std::function<void(dae::Scene* scene, glm::vec2 startPos)> CreateSecondaryPlayer;
		std::function<void(dae::Scene* scene)> CreateStage;
		std::function<void(dae::Scene* scene, glm::vec2 startPos)> CreateVersusStage;
		std::function<void(dae::Scene* scene)> CreateScore;

		//virtual void Update() override;
		//virtual void Render() const override;
		//virtual void FixedUpdate() override;

	};
}

