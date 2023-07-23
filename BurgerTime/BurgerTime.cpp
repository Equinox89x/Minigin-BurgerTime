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
#include "Commands.h"
#include "EndScreenComponent.h"
#include "Subject.h"
#include "AudioComponent.h"
#include "../3rdParty/SDL2_mixer-2.0.4/include/SDL_mixer.h"
#include <random>
#include "Observers.h"
#include "PlatformComponent.h"
#include "BurgerManager.h"
#include "BurgerComponent.h"


using namespace dae;

void CreateSelectorInput(dae::Scene* scene) {
	auto selector{ scene->GetGameObject(EnumStrings[Selector]) };
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_UP, 0 }, std::make_unique<CycleGameMode>(selector.get(), true));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_DOWN, 0 }, std::make_unique<CycleGameMode>(selector.get(), false));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_SPACE, 0 }, std::make_unique<StartGame>(selector.get()));
}

void CreateEndScreen(dae::Scene* /*scene*/) {
	
}

void CreateScore(dae::Scene* scene) {
	auto font = dae::ResourceManager::GetInstance().LoadFont("Emulogic-zrEw.ttf", 24);
	std::shared_ptr<GameObject> upText = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> upScoreText = std::make_shared<GameObject>();
	upScoreText->SetName(EnumStrings[ScoreHolder]);
	std::shared_ptr<GameObject> hiscoreText = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> hiscoreScoreText = std::make_shared<GameObject>();

	upText->AddComponent(std::make_unique<TextObjectComponent>("1UP", font));
	upScoreText->AddComponent(std::make_unique<TextObjectComponent>("0000", font));
	upScoreText->AddComponent(std::make_unique<ValuesComponent>(scene));
	auto observer{ std::make_shared<ScoreObserver>(scene) };
	upScoreText->GetComponent<ValuesComponent>()->AddObserver(observer);

	hiscoreText->AddComponent(std::make_unique<TextObjectComponent>("HI-SCORE", font));
	hiscoreScoreText->AddComponent(std::make_unique<TextObjectComponent>("00000", font));

	upText->GetComponent<TextObjectComponent>()->SetPosition(50, 10);
	upScoreText->GetComponent<TextObjectComponent>()->SetPosition(50, 40);
	hiscoreText->GetComponent<TextObjectComponent>()->SetPosition(200, 10);
	hiscoreScoreText->GetComponent<TextObjectComponent>()->SetPosition(200, 40);

	scene->Add(upText);
	scene->Add(upScoreText);
	scene->Add(hiscoreText);
	scene->Add(hiscoreScoreText);
}

void MakeValues(dae::Scene* /*scene*/) {

}

void MakeStageOfNr(dae::Scene* scene, Stages stageName) {
	std::shared_ptr<GameObject> burgerManager = std::make_shared<GameObject>();
	burgerManager->AddComponent(std::make_unique<TextureComponent>());

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
	std::vector<std::vector<int>> plates = stageItems[EnumStrings[Plates]];

	burgerManager->GetComponent<TextureComponent>()->SetTexture(name2);
	burgerManager->GetTransform()->AddTranslate(0, WindowBuffer);
	burgerManager->AddComponent(std::make_unique<BurgerManager>(scene));

	//burger maneger has to detect when another burger piece overlaps with another, if yes they fall down until they detect a platform. If that platform has a burger piece, that piece has to fall down as well.

	for (size_t i = 0; i < ladders.size(); i++)
	{
		SDL_Rect rect2{ ladders[i][0]-16, ladders[i][1] + WindowBuffer,ladders[i][2],ladders[i][3] };

		auto ladder2 = std::make_shared<GameObject>();
		ladder2->AddComponent(std::make_unique<PlatformComponent>(rect2));
		burgerManager->GetComponent<BurgerManager>()->AddLadder(rect2, ladder2.get());
		scene->Add(ladder2);
	}
	for (size_t i = 0; i < platforms.size(); i++)
	{
		SDL_Rect rect{ platforms[i][0], platforms[i][1] + WindowBuffer,platforms[i][2],platforms[i][3] };

		auto platform = std::make_shared<GameObject>();
		platform->AddComponent(std::make_unique<PlatformComponent>(rect));
		burgerManager->GetComponent<BurgerManager>()->AddPlatform(rect, platform.get());
		scene->Add(platform);
	}
	for (size_t i = 0; i < plates.size(); i++)
	{
		auto plate = std::make_shared<GameObject>();
		SDL_Rect rect{ plates[i][0], plates[i][1] + WindowBuffer,plates[i][2],plates[i][3] };
		plate->AddComponent(std::make_unique<PlatformComponent>(rect));
		burgerManager->GetComponent<BurgerManager>()->AddPlate(rect, plate.get());
		scene->Add(plate);
	}

	//auto observer{ std::make_shared<ScoreObserver>(scene) };
	for (size_t i = 0; i < pattiesTop.size(); i++)
	{
		GameObject* pattyTop = new GameObject();
		pattyTop->AddComponent(std::make_unique<TextureComponent>());
		pattyTop->GetComponent<TextureComponent>()->SetTexture("pattyTop.png");
		pattyTop->GetComponent<TextureComponent>()->Scale(3, 3);
		pattyTop->GetTransform()->Translate(static_cast<float>(pattiesTop[i][0]), static_cast<float>(pattiesTop[i][1])+ WindowBuffer);
		pattyTop->AddComponent(std::make_unique<BurgerComponent>(scene));	
		//pattyTop->GetComponent<BurgerComponent>()->AddObserver(observer);
		burgerManager->AddChild(pattyTop);


		GameObject* veggie = new GameObject();
		veggie->AddComponent(std::make_unique<TextureComponent>());
		veggie->GetComponent<TextureComponent>()->SetTexture("veggie.png");
		veggie->GetComponent<TextureComponent>()->Scale(3, 3);
		veggie->GetTransform()->Translate(static_cast<float>(veggies[i][0]), static_cast<float>(veggies[i][1]) + WindowBuffer);
		veggie->AddComponent(std::make_unique<BurgerComponent>(scene));
		//veggie->GetComponent<BurgerComponent>()->AddObserver(observer);
		burgerManager->AddChild(veggie);

		GameObject * burger = new GameObject();
		burger->AddComponent(std::make_unique<TextureComponent>());
		burger->GetComponent<TextureComponent>()->SetTexture("burger.png");
		burger->GetComponent<TextureComponent>()->Scale(3, 3);
		burger->GetTransform()->Translate(static_cast<float>(burgers[i][0]), static_cast<float>(burgers[i][1]) + WindowBuffer);
		burger->AddComponent(std::make_unique<BurgerComponent>(scene));
		//burger->GetComponent<BurgerComponent>()->AddObserver(observer);
		burgerManager->AddChild(burger);

		GameObject * pattyBottom = new GameObject();
		pattyBottom->AddComponent(std::make_unique<TextureComponent>());
		pattyBottom->GetComponent<TextureComponent>()->SetTexture("pattyBottom.png");
		pattyBottom->GetComponent<TextureComponent>()->Scale(3, 3);
		pattyBottom->GetTransform()->Translate(static_cast<float>(pattiesBottom[i][0]), static_cast<float>(pattiesBottom[i][1]) + WindowBuffer);
		pattyBottom->AddComponent(std::make_unique<BurgerComponent>(scene));
		//pattyBottom->GetComponent<BurgerComponent>()->AddObserver(observer);
		burgerManager->AddChild(pattyBottom);

		burgerManager->GetComponent<BurgerManager>()->AddBurger(pattyTop, pattyBottom, veggie, burger);
	}
	scene->Add(burgerManager);

	//enemies
	std::shared_ptr<GameObject> enemyHolder = std::make_shared<dae::GameObject>();
	enemyHolder->SetName(EnumStrings[EnemyHolder]);
	//enemyHolder->AddComponent(std::make_unique<EnemyManager>());
	scene->Add(enemyHolder);

	for (size_t i = 0; i < 4; i++)
	{
		GameObject* enemy = new GameObject();
		enemy->SetName(EnumStrings[Opposer]);
		enemy->AddComponent(std::make_unique<EnemyComponent>(scene, 200));
		enemy->AddComponent(std::make_unique<TextureComponent>());
		enemy->GetComponent<TextureComponent>()->SetTexture("moveDown.png");
		enemy->GetComponent<TextureComponent>()->Scale(3, 3);
		enemy->GetComponent<TextureComponent>()->SetNrOfFrames(3);
		enemy->GetComponent<TextureComponent>()->GetRect();
		enemy->GetTransform()->Translate(Margin, (WindowSizeY) - Margin * 5);
		enemyHolder->AddChild(enemy);
	}
}

void MakeMrHotdog(dae::Scene* scene) {
	std::shared_ptr<GameObject> enemyHolder = std::make_shared<dae::GameObject>();
	enemyHolder->SetName(EnumStrings[EnemyHolder]);
	scene->Add(enemyHolder);

	std::shared_ptr<GameObject> opposer = std::make_shared<GameObject>();
	opposer->SetName(EnumStrings[Opposer]);
	opposer->AddComponent(std::make_unique<TextureComponent>());

	opposer->GetComponent<TextureComponent>()->SetTexture("boss2.png");
	opposer->GetComponent<TextureComponent>()->SetName(EnumStrings[Enemy]);
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->Scale(3, 3);
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetNrOfFrames(2);
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->GetRect();
	opposer->GetComponent<TextureComponent>(EnumStrings[Enemy])->SetPosition((GameWindowSizeX) / 2 - Margin, WindowSizeY - Margin * 3);

	scene->Add(opposer);
	opposer->AddComponent(std::make_unique<MoveControllerComponent>(opposer->GetTransform()->GetPosition()));
}

void MakePlayer(dae::Scene* scene, std::string textureName, int id, bool /*isVersus*/) {
	auto playerName{ EnumStrings[PlayerGeneral] + std::to_string(id) };

	//Main Player
	std::shared_ptr<GameObject> mainPlayer = std::make_shared<dae::GameObject>();
	scene->Add(mainPlayer);
	mainPlayer->SetName(playerName);

	//Texture
	mainPlayer->AddComponent(std::make_unique<TextureComponent>());
	mainPlayer->GetComponent<TextureComponent>()->SetName(playerName);
	mainPlayer->GetComponent<TextureComponent>()->SetTexture(textureName);
	mainPlayer->GetComponent<TextureComponent>()->Scale(3, 3);
	//mainPlayer->GetComponent<TextureComponent>()->SetPosition((GameWindowSizeX / 2) - (Margin*2), WindowSizeY - ((Margin*3)+ WindowBuffer));
	mainPlayer->GetComponent<TextureComponent>()->SetNrOfFrames(3);
	mainPlayer->GetComponent<TextureComponent>()->GetRect();

	//bullets
	mainPlayer->AddComponent(std::make_unique<PlayerComponent>(scene));

	mainPlayer->GetTransform()->Translate((GameWindowSizeX / 2) - (Margin * 2), WindowSizeY - ((Margin * 3) + WindowBuffer));

	if (id == 0) {
		//Keyboard
		mainPlayer->AddComponent(std::make_unique<MoveKeyboardComponent>(mainPlayer->GetTransform()->GetPosition()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_w, id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), "moveUp.png", glm::vec3(0.f, -200.f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_s, id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), "moveDown.png", glm::vec3(0.f, 200.f, 0.0f)));
		 
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_a, id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), "moveLeft.png", glm::vec3(-200.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_d, id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), "moveRight.png", glm::vec3(200.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_w, id }, std::make_unique<StopMoveKeyboard>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_s, id }, std::make_unique<StopMoveKeyboard>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_a, id }, std::make_unique<StopMoveKeyboard>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_d, id }, std::make_unique<StopMoveKeyboard>(mainPlayer.get()));

		////Controller
		//mainPlayer->AddComponent(new MoveControllerComponent(mainPlayer->GetTransform()->GetPosition()));


		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadLeft, 0 }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(-600.f, 0.0f, 0.0f)));
		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadRight, 0 }, std::make_unique<MoveController>(mainPlayer->GetComponent<MoveControllerComponent>(), glm::vec3(600.f, 0.0f, 0.0f)));

		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadLeft, 0 }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
		//Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadRight, 0 }, std::make_unique<StopMoveController>(mainPlayer->GetComponent<MoveControllerComponent>()));
	}
	else {
		//Controller
		mainPlayer->AddComponent(std::make_unique<MoveControllerComponent>(mainPlayer->GetTransform()->GetPosition()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadUp, id }, std::make_unique<MoveController>(mainPlayer.get(), "moveUp.png", glm::vec3(-600.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadDown, id }, std::make_unique<MoveController>(mainPlayer.get(), "moveDown.png", glm::vec3(600.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadLeft, id }, std::make_unique<MoveController>(mainPlayer.get(), "moveLeft.png", glm::vec3(-600.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadRight, id }, std::make_unique<MoveController>(mainPlayer.get(), "moveRight.png", glm::vec3(600.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadUp, id }, std::make_unique<StopMoveController>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadDown, id }, std::make_unique<StopMoveController>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadLeft, id }, std::make_unique<StopMoveController>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadRight, id }, std::make_unique<StopMoveController>(mainPlayer.get()));
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
	upText1->AddComponent(std::make_unique<TextObjectComponent>("BURGER TIME", font));
	upText1->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 4, Margin);
	upText1->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });

	//Ti0tle 2
	std::shared_ptr<GameObject> upText2 = std::make_shared<dae::GameObject>();
	scene->Add(upText2);
	container->AddChild(upText2.get());
	upText2->AddComponent(std::make_unique<TextObjectComponent>("C DEGO 1982", font));
	upText2->GetComponent<TextObjectComponent>()->SetColor(SDL_Color{ 220,20,60 });
	upText2->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 4, Margin * 2);

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
	player1->AddComponent(std::make_unique<TextObjectComponent>("1 PLAYER", font));
	player2->AddComponent(std::make_unique<TextObjectComponent>("2 PLAYERS", font));
	player3->AddComponent(std::make_unique<TextObjectComponent>("VERSUS", font));
	player1->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - Margin * 3, 150);
	player2->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - Margin * 3, 150 + Margin);
	player3->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - Margin * 3, 150 + SubMargin);

	//game mode selector
	std::shared_ptr<GameObject> selector = std::make_shared<dae::GameObject>();
	selector->SetName(EnumStrings[Selector]);
	scene->Add(selector);
	container->AddChild(selector.get());
	selector->AddComponent(std::make_unique<ModeSelector>(scene, &MakeMrPepper, &MakeMrsSalt, &MakeStage, &MakeMrHotdog, &CreateScore));
	selector->AddComponent(std::make_unique<TextObjectComponent>(">", font));
	selector->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - SubMargin * 2, 150);
	CreateSelectorInput(scene);

	auto go{ std::make_shared<GameObject>() };
	go->SetName(EnumStrings[Global]);
	go->AddComponent(std::make_unique<AudioComponent>());
	//go->AddComponent(new MoveKeyboardComponent(go->GetTransform()->GetPosition()));
	go->AddComponent(std::make_unique<MoveKeyboardComponent>(go->GetTransform()->GetPosition()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_F1, 0 }, std::make_unique<Skip>(&MakeStageOfNr, scene));

	FileReader* file{ new FileReader("../Data/highscore.txt") };
	auto str{ file->ReadGameDataFile() };
	auto data{ file->ParseDataSimple(str, '+') };

	float yPos{ WindowSizeY / 2 };
	int i{ 1 };
	for (auto field : data)
	{
		std::shared_ptr<GameObject> scoreObj = std::make_shared<dae::GameObject>();
		std::shared_ptr<GameObject> scoreObj2 = std::make_shared<dae::GameObject>();
		std::string score{ std::to_string(i) + "  " + field.first };
		std::string score2{ std::any_cast<std::string>(field.second) + " PTS" };
		scoreObj->AddComponent(std::make_unique<TextObjectComponent>(score, font));
		scoreObj2->AddComponent(std::make_unique<TextObjectComponent>(score2, font));
		scoreObj->GetComponent<TextObjectComponent>()->SetPosition(Margin * 2, yPos);
		scoreObj2->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 + Margin, yPos);
		scene->Add(scoreObj);
		scene->Add(scoreObj2);
		i++;
		yPos += 50;
	}

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