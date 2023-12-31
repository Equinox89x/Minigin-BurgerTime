#include <stdexcept>
#define WIN32_LEAN_AND_MEAN 
#define _CRT_SECURE_NO_WARNINGS 
#include <windows.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Minigin.h"
#include "SceneManager.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Timer.h"
#include <chrono>
//#include <steam_api.h>
#include "Main.h"
#include "InputManager.h"
#include "Audio.h"
#include "ConsoleAudio.h"
#include "Locator.h"
#include "GameObject.h"

SDL_Window* g_window{};

void PrintSDLVersion()
{
	SDL_version version{};
	SDL_VERSION(&version);
	printf("We compiled against SDL version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	SDL_GetVersion(&version);
	printf("We are linking against SDL version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_IMAGE_VERSION(&version);
	printf("We compiled against SDL_image version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *IMG_Linked_Version();
	printf("We are linking against SDL_image version %u.%u.%u.\n",
		version.major, version.minor, version.patch);

	SDL_TTF_VERSION(&version)
	printf("We compiled against SDL_ttf version %u.%u.%u ...\n",
		version.major, version.minor, version.patch);

	version = *TTF_Linked_Version();
	printf("We are linking against SDL_ttf version %u.%u.%u.\n",
		version.major, version.minor, version.patch);
}

dae::Minigin::Minigin(const std::string &dataPath)
{
	PrintSDLVersion();
	
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) 
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}

	g_window = SDL_CreateWindow(
		"Programming 4 assignment",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		static_cast<int>(WindowSizeX),
		static_cast<int>(WindowSizeY),
		SDL_WINDOW_OPENGL
	);
	if (g_window == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}

	Renderer::GetInstance().Init(g_window);

	ResourceManager::GetInstance().Init(dataPath);

	Timer::GetInstance().Init();

	Locator::provide(new ConsoleAudio());
}

dae::Minigin::~Minigin()
{
	Renderer::GetInstance().Destroy();
	Locator::Quit();
	SDL_DestroyWindow(g_window);
	g_window = nullptr;
	SDL_Quit();
}

void dae::Minigin::Run(const std::function<void()>& load)
{
	load();

	Audio* audioService = new ConsoleAudio();
	Locator::provide(audioService);
	std::thread audioThread(&Audio::Update, audioService);

	auto& renderer = Renderer::GetInstance();
	auto& sceneManager = SceneManager::GetInstance();

	auto input = InputCollection(3);

	m_Running = true;
	constexpr float desired_FPS{ 144.0f };
	constexpr float frame_time{ 1000 / desired_FPS };

	Timer::GetInstance().Start();
	sceneManager.Initialize();
	sceneManager.PostInitialize();
	while (m_Running)
	{
		Timer::GetInstance().Update();

		input.ProcessInput();

		sceneManager.Update();
		renderer.Render();

		const float sleepTime{ frame_time - Timer::GetInstance().GetDeltaTime()};
		const auto& sleepDuration{ std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<float, std::milli>(sleepTime)) };
		if (sleepTime > 0)
			std::this_thread::sleep_for(sleepDuration);
	}

	audioThread.join();

	Timer::GetInstance().Stop();
}

void dae::Minigin::StopRunning()
{
	m_Running = false;
	Locator::getAudio()->StopAllSounds();
}