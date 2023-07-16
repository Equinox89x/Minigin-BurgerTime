#pragma once
#include <MoveKeyboardComponent.h>
#include <MoveControllerComponent.h>
#include <ValuesComponent.h>
#include <TextureComponent.h>
#include "ModeSelector.h"
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
		MoveKeyboard(MoveKeyboardComponent* const object, const glm::vec3& moveSpeed) : m_pObject(object), m_MoveSpeed(moveSpeed) {}
		void Execute() override
		{
			m_pObject->SetMoveSpeed(m_MoveSpeed);
		}
	private:
		MoveKeyboardComponent* m_pObject;
		glm::vec3 m_MoveSpeed;
	};

	class StopMoveKeyboard final : public Command
	{
	public:
		StopMoveKeyboard(MoveKeyboardComponent* const object) : m_pObject(object) {};
		void Execute() override
		{
			m_pObject->SetMoveSpeed(glm::vec3(0, 0, 0));
		}
	private:
		MoveKeyboardComponent* m_pObject;
	};
#pragma endregion

	class ChangeAnim final : public Command
	{
	public:
		ChangeAnim(TextureComponent* const object, std::string textureName) : m_pObject(object), m_TextureName(textureName) {}
		void Execute() override
		{
			m_pObject->SetTexture(m_TextureName);
			m_pObject->SetNrOfFrames(3);
		}
	private:
		TextureComponent* m_pObject;
		std::string m_TextureName;
	};

#pragma region Move Controller	
	class MoveController final : public Command
	{
	public:
		MoveController(MoveControllerComponent* const object, const glm::vec3& moveSpeed) : m_pObject(object), m_MoveSpeed(moveSpeed) {}
		void Execute() override
		{
			m_pObject->SetMoveSpeed(m_MoveSpeed);
		}
	private:
		MoveControllerComponent* m_pObject;
		glm::vec3 m_MoveSpeed;
	};

	class StopMoveController final : public Command
	{
	public:
		StopMoveController(MoveControllerComponent* const object) : m_pObject(object) {};
		void Execute() override
		{
			m_pObject->SetMoveSpeed(glm::vec3(0, 0, 0));
		}
	private:
		MoveControllerComponent* m_pObject;
	};
#pragma endregion

#pragma region values
	class IncreaseScore final : public Command
	{
	public:
		IncreaseScore(ValuesComponent* const object) : m_pObject(object) {}
		void Execute() override
		{
			m_pObject->IncreaseScore(100);
		}
	private:
		ValuesComponent* m_pObject;
	};


	class DownLives final : public Command
	{
	public:
		DownLives(ValuesComponent* const object) : m_pObject(object) {}
		void Execute() override
		{
			m_pObject->Damage();
		}
	private:
		ValuesComponent* m_pObject;
	};
#pragma endregion


#pragma region Menu
	class CycleGameMode final : public Command
	{
	public:
		CycleGameMode(ModeSelector* const object, bool isMoveUp) : m_pObject(object), m_IsMoveUp{ isMoveUp } {}
		void Execute() override
		{
			m_pObject->CycleGameMode(m_IsMoveUp);
		}
	private:
		ModeSelector* m_pObject;
		bool m_IsMoveUp{};
	};

	class StartGame final : public Command
	{
	public:
		StartGame(ModeSelector* const object, GameObject* menu) : m_pObject(object), m_pMenu{ menu } {}
		void Execute() override
		{
			m_pObject->StartGame(m_pMenu);
		}
	private:
		ModeSelector* m_pObject;
		GameObject* m_pMenu;
	};
#pragma endregion

#pragma region Skipping
	class Skip final : public Command
	{
	public:
		Skip(std::function<void(Scene*, Stages)> createStage, Scene* scene) : CreateStage(createStage), m_pScene{ scene } {}
		void Execute() override
		{
			auto enemies{ m_pScene->GetGameObject(EnumStrings[EnemyHolder]) };


			if (enemies)
				enemies->MarkForDestroy();

			auto children{ m_pScene->GetGameObject(EnumStrings[ScoreBoard])->GetChildren(EnumStrings[Life]) };
			for (size_t i = 0; i < 3; i++)
			{
				children[i]->SetIsHidden(false);
			}

			if (m_pScene->GetGameObject("Stage 1")) {
				CreateStage(m_pScene, Stages::Stage2);
				m_pScene->GetGameObject("Stage 1")->SetName("Stage 2");
			}
			else if (m_pScene->GetGameObject("Stage 2")) {
				CreateStage(m_pScene, Stages::Stage3);
				m_pScene->GetGameObject("Stage 2")->SetName("Stage 3");
			}
			else if (m_pScene->GetGameObject("Stage 3")) {
				CreateStage(m_pScene, Stages::Stage1);
				m_pScene->GetGameObject("Stage 3")->SetName("Stage 1");
			}
		}
	private:
		std::function<void(Scene*, Stages)> CreateStage;
		Scene* m_pScene;
	};
#pragma endregion
}