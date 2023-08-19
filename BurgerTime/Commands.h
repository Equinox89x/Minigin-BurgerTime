#pragma once
#include <MoveKeyboardComponent.h>
#include <MoveControllerComponent.h>
#include <ValuesComponent.h>
#include <TransformComponent.h>
#include <TextureComponent.h>
#include <GameObject.h>
#include "Scene.h"
#include "ModeSelector.h"
#include "BurgerManagerComponent.h"
#include <Command.h>

namespace dae
{
	/*class QuitGame final : public Command
		{
		public:
			QuitGame(dae::Minigin* pEngine) : m_pObject(pEngine) {}

			void Execute() override
			{
				m_pObject->StopRunning();
			}

		private:
			dae::Minigin* m_pObject;
		};*/

#pragma region Move Keyboard
	class MoveKeyboard final : public Command
	{
	public:
		MoveKeyboard(GameObject* object, std::string textureName, const glm::vec3& moveSpeed) : m_pObject(object), m_MoveSpeed(moveSpeed), m_TextureName(textureName) {}
		void Execute() override
		{			
				m_pObject->GetComponent<MoveKeyboardComponent>()->SetMoveSpeed(m_MoveSpeed);
				m_pObject->GetComponent<TextureComponent>()->SetTexture(m_TextureName);
				m_pObject->GetComponent<TextureComponent>()->SetNrOfFrames(3);
		}
	private:
		GameObject* m_pObject;
		glm::vec3 m_MoveSpeed;
		std::string m_TextureName;
	};

	class StopMoveKeyboard final : public Command
	{
	public:
		StopMoveKeyboard(GameObject* object) : m_pObject(object) {};
		void Execute() override
		{
			m_pObject->GetComponent<MoveKeyboardComponent>()->SetMoveSpeed(glm::vec3(0, 0, 0));
		}
	private:
		GameObject* m_pObject;
	};
#pragma endregion

#pragma region Move Controller	
	class MoveController final : public Command
	{
	public:
		MoveController(GameObject* object, std::string textureName, const glm::vec3& moveSpeed) : m_pObject(object), m_MoveSpeed(moveSpeed), m_TextureName(textureName) {}
		void Execute() override
		{
			auto player{ m_pObject->GetComponent<PlayerComponent>() };
			if (m_MoveSpeed.x != 0) {
				if (player->GetCanMoveHorizontally()) {
					m_pObject->GetComponent<MoveControllerComponent>()->SetMoveSpeed(m_MoveSpeed);
					m_pObject->GetComponent<TextureComponent>()->SetTexture(m_TextureName);
					m_pObject->GetComponent<TextureComponent>()->SetNrOfFrames(3);
				}
			}
			if (m_MoveSpeed.y != 0) {
				if (player->GetCanMoveVertically()) {
					m_pObject->GetComponent<MoveControllerComponent>()->SetMoveSpeed(m_MoveSpeed);
					m_pObject->GetComponent<TextureComponent>()->SetTexture(m_TextureName);
					m_pObject->GetComponent<TextureComponent>()->SetNrOfFrames(3);
				}
			}
		}
	private:
		GameObject* m_pObject;
		glm::vec3 m_MoveSpeed;
		std::string m_TextureName;
	};

	class StopMoveController final : public Command
	{
	public:
		StopMoveController(GameObject* object) : m_pObject(object) {};
		void Execute() override
		{
			m_pObject->GetComponent<MoveControllerComponent>()->SetMoveSpeed(glm::vec3(0, 0, 0));
		}
	private:
		GameObject* m_pObject;
	};
#pragma endregion

#pragma region values
	class IncreaseScore final : public Command
	{
	public:
		IncreaseScore(GameObject* object) : m_pObject(object) {}
		void Execute() override
		{

			m_pObject->GetComponent<ValuesComponent>()->IncreaseScore(100);
		}
	private:
		GameObject* m_pObject;
	};


	class DownLives final : public Command
	{
	public:
		DownLives(GameObject* const object) : m_pObject(object) {}
		void Execute() override
		{
			m_pObject->GetComponent<ValuesComponent>()->Damage();
		}
	private:
		GameObject* m_pObject;
	};
#pragma endregion


#pragma region Menu
	class CycleGameMode final : public Command
	{
	public:
		CycleGameMode(GameObject* object, bool isMoveUp) : m_pObject(object), m_IsMoveUp{ isMoveUp } {}
		void Execute() override
		{
			m_pObject->GetComponent<ModeSelector>()->CycleGameMode(m_IsMoveUp);
		}
	private:
		GameObject* m_pObject;
		bool m_IsMoveUp{};
	};

	class StartGame final : public Command
	{
	public:
		StartGame(GameObject* object) : m_pObject(object) {}
		void Execute() override
		{
			m_pObject->GetComponent<ModeSelector>()->StartGame(m_pObject->GetParent());
		}
	private:
		GameObject* m_pObject;
	};
#pragma endregion

#pragma region Skipping
	class Skip final : public Command
	{
	public:
		Skip(std::function<void(Scene*, Stages, bool)> createStage, std::function <void(Scene* , glm::vec2)>createMainPlayer, Scene* scene) : CreateStage(createStage), CreateMainPlayer{ createMainPlayer }, m_pScene{scene} {}
		void Execute() override
		{
			auto enemies{ m_pScene->GetGameObject(EnumStrings[EnemyHolder]) };
			auto player{ m_pScene->GetGameObject(EnumStrings[Player0]) };
			auto scoreHolder{m_pScene->GetGameObject(EnumStrings[ScoreHolder])};
			auto burgerManager{m_pScene->GetGameObject(EnumStrings[BurgerManager])};

			if (burgerManager) {
				burgerManager->GetComponent<BurgerManagerComponent>()->DeleteItems();
			}

			if (enemies)
				enemies->MarkForDestroy();

			if (scoreHolder) {
				auto children{ scoreHolder->GetChildren(EnumStrings[Life]) };
				if (children.size() > 0) {
					for (size_t i = 0; i < 3; i++)
					{
						children[i]->SetIsHidden(false);
					}
				}
			}

			if (player) m_pScene->Remove(player);

			if (burgerManager) {
				if (burgerManager->GetComponent<TextureComponent>()->GetName() == "Stage 3") {
					m_pScene->Remove(burgerManager);
					CreateStage(m_pScene, Stages::Stage1, false);
					CreateMainPlayer(m_pScene, glm::vec2{ (WindowSizeX / 2) - (Margin), WindowSizeY - ((Margin * 3) + WindowBuffer) });
				}
				else if (burgerManager->GetComponent<TextureComponent>()->GetName() == "Stage 1") {
					m_pScene->Remove(burgerManager);
					CreateStage(m_pScene, Stages::Stage2, false);
					CreateMainPlayer(m_pScene, glm::vec2{ (WindowSizeX / 2) - (Margin - 10), WindowSizeY - ((Margin * 1.5f) + WindowBuffer + 5) });
				}
				else if (burgerManager->GetComponent<TextureComponent>()->GetName() == "Stage 2") {
					m_pScene->Remove(burgerManager);
					CreateStage(m_pScene, Stages::Stage3, false);
					CreateMainPlayer(m_pScene, glm::vec2{ (WindowSizeX / 2) - (Margin-10), WindowSizeY - (WindowBuffer + 10) });
				}
			}
		}
	private:
		std::function<void(Scene*, Stages, bool)> CreateStage;
		std::function<void(Scene*, glm::vec2)> CreateMainPlayer;
		Scene* m_pScene;
	};
#pragma endregion
}