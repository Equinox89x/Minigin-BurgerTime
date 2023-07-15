#include <SDL.h>

#if _DEBUG
// ReSharper disable once CppUnusedIncludeDirective
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include "Minigin.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "TextObjectComponent.h"
#include "FPSCounterComponent.h"
#include "TextureComponent.h"
#include "GameObject.h"
#include "Scene.h"
#include "FileReader.h"
#include "RotatorComponent.h"
#include "MoveKeyboardComponent.h"
#include "MoveControllerComponent.h"
#include "imgui.h"
#include "imgui_plot.h"
#include <chrono>
#include "InputManager.h"
#include "ValuesComponent.h"
#include <sstream>
#include "Main.h"
#include "EnemyComponent.h"
#include "OverlapComponent.h"
#include "PlayerComponent.h"
#include "CommandProject.h"
#include "EndScreenComponent.h"
#include "Callback.h"
#include "AudioComponent.h"
#include "../3rdParty/SDL2_mixer-2.0.4/include/SDL_mixer.h"
#include <random>


using namespace dae;

void CreateSelectorInput(dae::Scene* scene) {
	auto selector{ scene->GetGameObject(EnumStrings[Selector]) };
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_UP, 0 }, std::make_unique<CycleGameMode>(selector->GetComponent<ModeSelector>(), true));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_DOWN, 0 }, std::make_unique<CycleGameMode>(selector->GetComponent<ModeSelector>(), false));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_SPACE, 0 }, std::make_unique<StartGame>(selector->GetComponent<ModeSelector>(), selector->GetParent()));
}

void CreateEndScreen(dae::Scene* /*scene*/) {
	
}

void CreateScore(dae::Scene* /*scene*/) {
	
}

void MakeValues(dae::Scene* /*scene*/) {

}

void MakeStageOfNr(dae::Scene* scene, Stages stageName) {
	std::shared_ptr<GameObject> burgerManager = std::make_shared<GameObject>();
	burgerManager->AddComponent(new TextureComponent());

	std::string name;
	std::string name2;
	switch (stageName)
	{
	case Stages::Stage1:
		name = "Stage 1";
		name2 = "Stage1.png";
		break;
	case Stages::Stage2:
		name = "Stage 2";
		name2 = "Stage2.png";
		break;
	case Stages::Stage3:
		name = "Stage 3";
		name2 = "Stage3.png";
		break;
	default:
		break;
	}

	FileReader* file{ new FileReader("../Data/galagamap.txt") };
	auto str{ file->ReadGameDataFile() };
	auto data{ file->ParseData(str, '+') };
	auto stageItems = std::any_cast<std::map<std::string, std::vector<std::vector<int>>>>(data[name]);
	std::vector<std::vector<int>> ladders = stageItems[EnumStrings[Ladders]];
	std::vector<std::vector<int>> platforms = stageItems[EnumStrings[Platforms]];
	std::vector<std::vector<int>> pattiesTop = stageItems[EnumStrings[PattyTop]];
	std::vector<std::vector<int>> pattiesBottom = stageItems[EnumStrings[PattyBottom]];
	std::vector<std::vector<int>> veggies = stageItems[EnumStrings[Veggie]];
	std::vector<std::vector<int>> burgers = stageItems[EnumStrings[Burger]];

	burgerManager->GetComponent<TextureComponent>()->SetTexture(name2);
	//for (size_t i = 0; i < ladders.size(); i++)
	//{

	//}
	//for (size_t i = 0; i < platforms.size(); i++)
	//{

	//}
	for (size_t i = 0; i < pattiesTop.size(); i++)
	{
		GameObject* pattyTop = new GameObject();
		pattyTop->AddComponent(new TextureComponent());
		pattyTop->GetComponent<TextureComponent>()->SetTexture("pattyTop.png");
		pattyTop->GetComponent<TextureComponent>()->Scale(3, 3);
		pattyTop->GetComponent<TransformComponent>()->Translate(static_cast<float>(pattiesTop[i][0]), static_cast<float>(pattiesTop[i][1]));
		//pattyTop->AddComponent(new BurgerComponent("pattyTop", "png"));		
		burgerManager->AddChild(pattyTop);

		GameObject* veggie = new GameObject();
		veggie->AddComponent(new TextureComponent());
		veggie->GetComponent<TextureComponent>()->SetTexture("veggie.png");
		veggie->GetComponent<TextureComponent>()->Scale(3, 3);
		veggie->GetComponent<TransformComponent>()->Translate(static_cast<float>(veggies[i][0]), static_cast<float>(veggies[i][1]));
		//veggie->AddComponent(new BurgerComponent("veggie", "png"));
		burgerManager->AddChild(veggie);

		GameObject* burger = new GameObject();
		burger->AddComponent(new TextureComponent());
		burger->GetComponent<TextureComponent>()->SetTexture("burger.png");
		burger->GetComponent<TextureComponent>()->Scale(3, 3);
		burger->GetComponent<TransformComponent>()->Translate(static_cast<float>(burgers[i][0]), static_cast<float>(burgers[i][1]));
		//burger->AddComponent(new BurgerComponent("burger", "png"));
		burgerManager->AddChild(burger);

		GameObject* pattyBottom = new GameObject();
		pattyBottom->AddComponent(new TextureComponent());
		pattyBottom->GetComponent<TextureComponent>()->SetTexture("pattyBottom.png");
		pattyBottom->GetComponent<TextureComponent>()->Scale(3, 3);
		pattyBottom->GetComponent<TransformComponent>()->Translate(static_cast<float>(pattiesBottom[i][0]), static_cast<float>(pattiesBottom[i][1]));
		//pattyBottom->AddComponent(new BurgerComponent("pattyBottom", "png"));
		burgerManager->AddChild(pattyBottom);

		//burgerManager->AddBurger(pattyTop, veggie, burger, pattyBottom);
		//scene->Add(pattyTop);
		//scene->Add(veggie);
		//scene->Add(burger);
		//scene->Add(pattyBottom);
	}
	scene->Add(burgerManager);
}

void MakeMrHotdog(dae::Scene* scene) {
	std::shared_ptr<GameObject> enemyHolder = std::make_shared<dae::GameObject>();
	enemyHolder->SetName(EnumStrings[EnemyHolder]);
	scene->Add(enemyHolder);

	std::shared_ptr<GameObject> opposer = std::make_shared<GameObject>();
	opposer->SetName(EnumStrings[Opposer]);
	opposer->AddComponent(new TextureComponent());

	Callback* callback = new Callback();
	callback->AddObserver(new GameOverObserver(&CreateEndScreen, scene));

	opposer->GetComponent<TextureComponent>()->SetTexture("boss2.png");
	opposer->GetComponent<TextureComponent>()->SetName(EnumStrings[Enemy]);
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->Scale(3, 3);
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetNrOfFrames(2);
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->GetRect();
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetPosition((GameWindowSizeX) / 2 - Margin, Margin*2);

	scene->Add(opposer);
	opposer->AddComponent(new MoveControllerComponent(opposer->GetTransform()->GetPosition()));
}

void MakePlayer(dae::Scene* scene, std::string textureName, int id, bool /*isVersus*/) {
	auto playerName{ EnumStrings[PlayerGeneral] + std::to_string(id) };

	//Main Player
	std::shared_ptr<GameObject> mainPlayer = std::make_shared<dae::GameObject>();
	scene->Add(mainPlayer);
	mainPlayer->SetName(playerName);

	//Texture
	mainPlayer->AddComponent(new TextureComponent());
	mainPlayer->GetComponent<TextureComponent>()->SetName(playerName);
	mainPlayer->GetComponent<TextureComponent>()->SetTexture(textureName);
	mainPlayer->GetComponent<TextureComponent>()->Scale(3, 3);
	mainPlayer->GetComponent<TextureComponent>()->SetPosition((GameWindowSizeX) / 2 - Margin, WindowSizeY - Margin*3);
	mainPlayer->GetComponent<TextureComponent>()->SetNrOfFrames(3);
	mainPlayer->GetComponent<TextureComponent>()->GetRect();

	//bullets
	mainPlayer->AddComponent(new PlayerComponent(scene));

	if (id == 0) {
		//Keyboard
		mainPlayer->AddComponent(new MoveKeyboardComponent(mainPlayer->GetTransform()->GetPosition()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_w, id }, std::make_unique<ChangeAnim>(mainPlayer->GetComponent<TextureComponent>(), "moveUp.png"));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_w, id }, std::make_unique<MoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>(), glm::vec3(0.f, -300.f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_s, id }, std::make_unique<ChangeAnim>(mainPlayer->GetComponent<TextureComponent>(), "moveDown.png"));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_s, id }, std::make_unique<MoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>(), glm::vec3(0.f, 300.f, 0.0f)));
		 
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_a, id }, std::make_unique<ChangeAnim>(mainPlayer->GetComponent<TextureComponent>(), "moveLeft.png"));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_a, id }, std::make_unique<MoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>(), glm::vec3(-300.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_d, id }, std::make_unique<ChangeAnim>(mainPlayer->GetComponent<TextureComponent>(), "moveRight.png"));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_d, id }, std::make_unique<MoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>(), glm::vec3(300.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_w, id }, std::make_unique<StopMoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_s, id }, std::make_unique<StopMoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_a, id }, std::make_unique<StopMoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_d, id }, std::make_unique<StopMoveKeyboard>(mainPlayer->GetComponent<MoveKeyboardComponent>()));

		////Controller
		//mainPlayer->AddComponent(new MoveControllerComponent(mainPlayer->GetTransform()->GetPosition()));


		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadLeft, 0 }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(-600.f, 0.0f, 0.0f)));
		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadRight, 0 }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(600.f, 0.0f, 0.0f)));

		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadLeft, 0 }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadRight, 0 }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
	}
	else {
		//Controller
		mainPlayer->AddComponent(new MoveControllerComponent(mainPlayer->GetTransform()->GetPosition()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadUp, id }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(-600.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadDown, id }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(600.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadLeft, id }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(-600.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadRight, id }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(600.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadUp, id }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadDown, id }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadLeft, id }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadRight, id }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
	}
	
	MakeValues(scene);
}

void MakeMrPepper(dae::Scene* scene, bool isVersus) {
	MakePlayer(scene, "moveUp.png", 0, isVersus);
}

void MakeMrsSalt(dae::Scene* scene) {
	MakePlayer(scene, "moveUp.png", 1, false);
}

void MakeStage(dae::Scene* scene) {
	MakeStageOfNr(scene, Stages::Stage1);
}

void MakeVersusStage(dae::Scene* scene) {
	std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();
	scene->Add(container);
}

void MakeMainMenu(dae::Scene* scene) {
	std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();
	container->SetName(EnumStrings[MainMenu]);
	scene->Add(container);

	auto font = dae::ResourceManager::GetInstance().LoadFont("Emulogic-zrEw.ttf", 24);

	//Title
	std::shared_ptr<GameObject> upText1 = std::make_shared<dae::GameObject>();
	scene->Add(upText1);
	container->AddChild(upText1.get());
	upText1->AddComponent(new TextObjectComponent("BURGER TIME", font));
	upText1->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX/2, Margin);
	upText1->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });

	//Ti0tle 2
	std::shared_ptr<GameObject> upText2 = std::make_shared<dae::GameObject>();
	scene->Add(upText2);
	container->AddChild(upText2.get());
	upText2->AddComponent(new TextObjectComponent("C DEGO 1982", font));
	upText2->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });
	upText2->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX/2, Margin*2);

	//game mode selection
	std::shared_ptr<GameObject> player1 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> player2 = std::make_shared<dae::GameObject>();
	std::shared_ptr<GameObject> player3 = std::make_shared<dae::GameObject>();
	scene->Add(player1);
	scene->Add(player2);
	scene->Add(player3);
	container->AddChild(player1.get());
	container->AddChild(player2.get());
	container->AddChild(player3.get());
	player1->AddComponent(new TextObjectComponent("1 PLAYER", font));
	player2->AddComponent(new TextObjectComponent("2 PLAYERS", font));
	player3->AddComponent(new TextObjectComponent("VERSUS", font));
	player1->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - Margin * 3, WindowSizeY / 2);
	player2->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - Margin * 3, WindowSizeY / 2 + Margin);
	player3->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - Margin * 3, WindowSizeY / 2 + SubMargin);

	//game mode selector
	std::shared_ptr<GameObject> selector = std::make_shared<dae::GameObject>();
	selector->SetName(EnumStrings[Selector]);
	scene->Add(selector);
	container->AddChild(selector.get());
	selector->AddComponent(new ModeSelector(scene, &MakeMrPepper, &MakeMrsSalt, &MakeStage, &MakeMrHotdog, &CreateScore));
	selector->AddComponent(new TextObjectComponent(">", font));
	selector->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - SubMargin * 2, WindowSizeY / 2);
	CreateSelectorInput(scene);

	auto go{ std::make_shared<GameObject>() };
	go->SetName(EnumStrings[Global]);
	go->AddComponent(new AudioComponent());
	//go->AddComponent(new MoveKeyboardComponent(go->GetTransform()->GetPosition()));
	go->AddComponent(new MoveKeyboardComponent(go->GetTransform()->GetPosition()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_F1, 0 }, std::make_unique<Skip>(&MakeStageOfNr, scene));
	scene->Add(go);
}

void load()
{
	auto& scene = dae::SceneManager::GetInstance().CreateScene("Galaga");

	MakeMainMenu(&scene);
}

int main(int, char* []) {
	dae::Minigin engine("../Data/");
	engine.Run(load);

	return 0;
}