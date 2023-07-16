#pragma once
#include "Singleton.h"
#include <tuple>
#include <map>
#include <memory>
#include "Command.h"
#include <Windows.h>
#include <Xinput.h>

#define MAX_PLAYERS 2

namespace dae
{

	enum class ControllerButton
	{
		ButtonA = 0x1000,
		ButtonB = 0x2000,
		ButtonX = 0x4000,
		ButtonY = 0x8000,
		DpadUp = 0x0001,
		DpadDown = 0x0002,
		DpadLeft = 0x0004,
		DpadRight = 0x0008,
		LeftThumb = 0x0040,
		RightThumb = 0x0080,
		LeftShoulder = 0x0100,
		RightShoulder = 0x0200,
		Start = 0x0010,
		Back = 0x0020
	};

	enum class ButtonStates
	{
		BUTTON_DOWN,
		BUTTON_PRESSED,
		BUTTON_UP
	};

	class Input final : public Singleton<Input>
	{
	private:
		using ControllerKey = std::tuple<ButtonStates, ControllerButton, int>;
		using KeyboardKey = std::tuple<ButtonStates, SDL_KeyCode, int>;
		using ControllerCommandsMap = std::map<ControllerKey, std::unique_ptr<Command>>;
		using KeyboardCommandsMap = std::map<KeyboardKey, std::unique_ptr<Command>>;

		ControllerCommandsMap m_consoleCommands{};
		KeyboardCommandsMap m_KeyboardCommands{};
		bool m_Clear{};

	public:
		void ClearKeys();
		bool GetClear() const { return m_Clear; };
		void SetClear(bool clear) { m_Clear = clear; };
		void BindKey(ControllerKey key, std::unique_ptr<Command> c);
		void BindKey(KeyboardKey key, std::unique_ptr<Command> c);

		const ControllerCommandsMap& GetConsoleCommands()
		{
			return m_consoleCommands;
		};

		const KeyboardCommandsMap& GetKeyboardCommands()
		{
			return m_KeyboardCommands;
		};
	};

	class InputManager
	{
		class InputManagerImpl;
		InputManagerImpl* pImpl;

	public:
		InputManager(int controllerIndex);
		~InputManager();
		InputManager(const InputManager& other) = delete;
		InputManager(InputManager&& other) = delete;
		InputManager& operator=(const InputManager& other) = delete;
		InputManager& operator=(InputManager&& other) = delete;

		//void ProcessInput();
		bool IsPressed(ControllerButton button) const;
		bool IsDownThisFrame(ControllerButton button) const;
		bool IsUpThisFrame(ControllerButton button) const;
		void HandleInput();
	private:

	};

	class InputCollection {

	public:
		InputCollection(int nrOfControllers) : m_NrOfControllers{nrOfControllers-1} {
			for (int i = 0; i < m_NrOfControllers; i++)
			{
				m_Inputs.push_back(std::make_unique<InputManager>(i));
			} 
		};
		~InputCollection() {
			for (int i = 0; i < m_NrOfControllers; i++)
			{
				m_Inputs[i].release();
			}
		};
		InputCollection(const InputCollection& other) = delete;
		InputCollection(InputCollection&& other) = delete;
		InputCollection& operator=(const InputCollection& other) = delete;
		InputCollection& operator=(InputCollection&& other) = delete;

		void ProcessInput() {
			for (int i = 0; i < m_NrOfControllers; i++)
			{
				m_Inputs[i]->HandleInput();
			}
		}

	private:
		int m_NrOfControllers{ 0 };
		std::vector<std::unique_ptr<InputManager>> m_Inputs;
	};
}