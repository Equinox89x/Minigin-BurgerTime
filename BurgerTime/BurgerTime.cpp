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
#include "BurgerManagerComponent.h"
#include "BurgerComponent.h"


using namespace dae;

void CreateSelectorInput(dae::Scene* scene) {
	auto selector{ scene->GetGameObject(EnumStrings[Selector]) };
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_UP, 0 }, std::make_unique<CycleGameMode>(selector.get(), true));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_DOWN, 0 }, std::make_unique<CycleGameMode>(selector.get(), false));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_SPACE, 0 }, std::make_unique<StartGame>(selector.get()));

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, ControllerButton::ButtonY, 0 }, std::make_unique<CycleGameMode>(selector.get(), true));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, ControllerButton::ButtonA, 0 }, std::make_unique<CycleGameMode>(selector.get(), false));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, ControllerButton::ButtonX, 0 }, std::make_unique<StartGame>(selector.get()));
}

void CreateEndScreen(dae::Scene* /*scene*/) {
	
}

void CreateScore(dae::Scene* scene) {
	scene->GetGameObject(EnumStrings[Selector])->SetIsHidden(true);

	std::shared_ptr<GameObject> scoreHolder = std::make_shared<GameObject>();
	scoreHolder->AddComponent(std::make_unique<ValuesComponent>(scene));
	scoreHolder->SetName(EnumStrings[ScoreHolder]);
	auto observer{ std::make_shared<ScoreObserver>(scene) };
	scoreHolder->GetComponent<ValuesComponent>()->AddObserver(observer);
	scoreHolder->GetComponent<ValuesComponent>()->SetLives(3);
	auto observer2{ std::make_shared<HealthObserver>(scene) };
	scoreHolder->GetComponent<ValuesComponent>()->AddObserver(observer2);	
	auto observer3{ std::make_shared<GameOverObserver>(&CreateEndScreen, scene) };
	scoreHolder->GetComponent<ValuesComponent>()->AddObserver(observer3);
	scoreHolder->GetComponent<ValuesComponent>()->SetSalt(5);
	auto observer4{ std::make_shared<PepperObserver>(scene) };
	scoreHolder->GetComponent<ValuesComponent>()->AddObserver(observer4);


	auto font = dae::ResourceManager::GetInstance().LoadFont("Emulogic-zrEw.ttf", 24);
	std::shared_ptr<GameObject> upText = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> upScoreText = std::make_shared<GameObject>();
	upScoreText->SetName(EnumStrings[Names::Score]);
	std::shared_ptr<GameObject> hiscoreText = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> hiscoreScoreText = std::make_shared<GameObject>();

	std::shared_ptr<GameObject> saltText = std::make_shared<GameObject>();
	std::shared_ptr<GameObject> saltScoreText = std::make_shared<GameObject>();
	saltScoreText->SetName(EnumStrings[Names::Pepper]);

	upText->AddComponent(std::make_unique<TextObjectComponent>("1UP", font));
	upScoreText->AddComponent(std::make_unique<TextObjectComponent>("0000", font));

	hiscoreText->AddComponent(std::make_unique<TextObjectComponent>("HI-SCORE", font));
	hiscoreScoreText->AddComponent(std::make_unique<TextObjectComponent>("00000", font));

	saltText->AddComponent(std::make_unique<TextObjectComponent>("Peppers", font));
	saltScoreText->AddComponent(std::make_unique<TextObjectComponent>("5", font));

	upText->GetComponent<TextObjectComponent>()->SetPosition(50, 10);
	upScoreText->GetComponent<TextObjectComponent>()->SetPosition(50, 40);
	hiscoreText->GetComponent<TextObjectComponent>()->SetPosition(200, 10);
	hiscoreScoreText->GetComponent<TextObjectComponent>()->SetPosition(200, 40);

	saltText->GetComponent<TextObjectComponent>()->SetPosition(500, 10);
	saltScoreText->GetComponent<TextObjectComponent>()->SetPosition(500, 40);

	scene->Add(upText);
	scene->Add(upScoreText);
	scene->Add(hiscoreText);
	scene->Add(hiscoreScoreText);
	scene->Add(saltScoreText);
	scene->Add(saltText);
	scoreHolder->AddChild(upText.get());
	scoreHolder->AddChild(upScoreText.get());
	scoreHolder->AddChild(hiscoreText.get());
	scoreHolder->AddChild(hiscoreScoreText.get());
	scoreHolder->AddChild(saltScoreText.get());
	scoreHolder->AddChild(saltText.get());

	auto height{ 0.f };
	for (size_t i = 0; i < 3; i++)
	{
		auto life = std::make_shared<GameObject>();
		life->SetName(EnumStrings[Life]);
		life->AddComponent(std::make_unique<TextureComponent>());
		life->GetComponent<TextureComponent>()->SetTexture("life.png");
		life->GetComponent<TextureComponent>()->Scale(3,3);
		life->GetComponent<TextureComponent>()->SetNrOfFrames(1);
		life->GetTransform()->Translate(Margin/4, WindowSizeY - Margin);
		life->GetTransform()->AddTranslate(0, height);
		height -= Margin/1.5f;
		scoreHolder->AddChild(life.get());
		scene->Add(life);
	}

	scene->Add(scoreHolder);
}

void MakeValues(dae::Scene* /*scene*/) {

}

void CreateObjects(dae::Scene* scene, BurgerManagerComponent* burgerManagerComp, std::shared_ptr<GameObject> burgerManager, bool isVersus, std::string name) {
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
	//burger maneger has to detect when another burger piece overlaps with another, if yes they fall down until they detect a platform. 
	//If that platform has a burger piece, that piece has to fall down as well.
	for (size_t i = 0; i < ladders.size(); i++)
	{
		SDL_Rect rect2{ ladders[i][0] - 16, ladders[i][1] + WindowBuffer,ladders[i][2],ladders[i][3] };

		auto ladder2 = std::make_shared<GameObject>();
		ladder2->AddComponent(std::make_unique<PlatformComponent>(rect2));
		burgerManagerComp->AddLadder(rect2, ladder2.get());
		ladder2->SetName(EnumStrings[Ladders]);
		scene->Add(ladder2);
	}
	for (size_t i = 0; i < platforms.size(); i++)
	{
		SDL_Rect rect{ platforms[i][0], platforms[i][1] + WindowBuffer,platforms[i][2],platforms[i][3] };

		auto platform = std::make_shared<GameObject>();
		platform->AddComponent(std::make_unique<PlatformComponent>(rect));
		burgerManagerComp->AddPlatform(rect, platform.get());
		platform->SetName(EnumStrings[Platforms]);
		scene->Add(platform);
	}
	for (size_t i = 0; i < plates.size(); i++)
	{
		auto plate = std::make_shared<GameObject>();
		SDL_Rect rect{ plates[i][0], plates[i][1] + WindowBuffer,plates[i][2],plates[i][3] };
		plate->AddComponent(std::make_unique<PlatformComponent>(rect));
		burgerManagerComp->AddPlate(rect, plate.get());
		plate->SetName(EnumStrings[Plates]);
		scene->Add(plate);
	}

	for (size_t i = 0; i < pattiesTop.size(); i++)
	{
		GameObject* pattyTop = new GameObject();
		pattyTop->AddComponent(std::make_unique<TextureComponent>());
		pattyTop->GetComponent<TextureComponent>()->SetTexture("pattyTop.png");
		pattyTop->GetComponent<TextureComponent>()->Scale(3, 3);
		pattyTop->GetTransform()->Translate(static_cast<float>(pattiesTop[i][0]), static_cast<float>(pattiesTop[i][1]) + WindowBuffer);
		pattyTop->AddComponent(std::make_unique<BurgerComponent>(scene));
		pattyTop->SetName(EnumStrings[PattyTop]);
		burgerManager->AddChild(pattyTop);

		GameObject* veggie = new GameObject();
		if (veggies.size() != 0) {
			veggie->AddComponent(std::make_unique<TextureComponent>());
			veggie->GetComponent<TextureComponent>()->SetTexture("veggie.png");
			veggie->GetComponent<TextureComponent>()->Scale(3, 3);
			veggie->GetTransform()->Translate(static_cast<float>(veggies[i][0]), static_cast<float>(veggies[i][1]) + WindowBuffer);
			veggie->AddComponent(std::make_unique<BurgerComponent>(scene));
			veggie->SetName(EnumStrings[Veggie]);
			burgerManager->AddChild(veggie);
		}
		else {
			delete veggie;
			veggie = nullptr;
		}

		GameObject* burger = new GameObject();
		burger->AddComponent(std::make_unique<TextureComponent>());
		burger->GetComponent<TextureComponent>()->SetTexture("burger.png");
		burger->GetComponent<TextureComponent>()->Scale(3, 3);
		burger->GetTransform()->Translate(static_cast<float>(burgers[i][0]), static_cast<float>(burgers[i][1]) + WindowBuffer);
		burger->AddComponent(std::make_unique<BurgerComponent>(scene));
		burger->SetName(EnumStrings[Burger]);
		burgerManager->AddChild(burger);

		GameObject* pattyBottom = new GameObject();
		pattyBottom->AddComponent(std::make_unique<TextureComponent>());
		pattyBottom->GetComponent<TextureComponent>()->SetTexture("pattyBottom.png");
		pattyBottom->GetComponent<TextureComponent>()->Scale(3, 3);
		pattyBottom->GetTransform()->Translate(static_cast<float>(pattiesBottom[i][0]), static_cast<float>(pattiesBottom[i][1]) + WindowBuffer);
		pattyBottom->AddComponent(std::make_unique<BurgerComponent>(scene));
		pattyBottom->SetName(EnumStrings[PattyBottom]);
		burgerManager->AddChild(pattyBottom);

		burgerManagerComp->AddBurger(pattyTop, pattyBottom, veggie, burger);
	}

	if (!isVersus) {
		//enemies
		std::shared_ptr<GameObject> enemyHolder = std::make_shared<dae::GameObject>();
		enemyHolder->SetName(EnumStrings[EnemyHolder]);
		scene->Add(enemyHolder);

		std::vector<std::string> names{ "hotdog","egg", "pickle", "hotdog" };
		std::vector<EnemyType> types{ EnemyType::Hotdog, EnemyType::Egg, EnemyType::Pickle, EnemyType::Hotdog };
		glm::vec2 pos{ Margin, (WindowSizeY)-Margin * 5 };
		for (size_t i = 0; i < 4; i++)
		{
			GameObject* enemy = new GameObject();
			enemy->SetName(EnumStrings[Enemy]);
			enemy->AddComponent(std::make_unique<EnemyComponent>(scene, types[i], pos));
			enemy->AddComponent(std::make_unique<TextureComponent>());
			enemy->GetComponent<TextureComponent>()->SetTexture(names[i] + "Down.png");
			enemy->GetComponent<TextureComponent>()->Scale(3, 3);
			enemy->GetComponent<TextureComponent>()->SetNrOfFrames(2);
			enemy->GetComponent<TextureComponent>()->GetRect();
			enemy->GetTransform()->Translate(pos);
			enemyHolder->AddChild(enemy);
		}
	}
}

void ReloadStageOfNr(dae::Scene* scene, Stages stageName, bool isVersus) {
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

	auto burgerManager{ scene->GetGameObject(EnumStrings[BurgerManager]) };
	auto burgerManagerComp{ burgerManager->GetComponent<BurgerManagerComponent>()};
	FileReader* file {new FileReader("../Data/galagamap.txt")};
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
	burgerManager->GetComponent<TextureComponent>()->SetName(name);
	burgerManager->GetComponent<TextureComponent>()->SetName(name);

	CreateObjects(scene, burgerManagerComp, burgerManager, isVersus, name);

	scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayMenuSound(true);
}
void MakeStageOfNr(dae::Scene* scene, Stages stageName, bool isVersus) {

	std::shared_ptr<GameObject> burgerManager = std::make_shared<GameObject>();
	burgerManager->AddComponent(std::make_unique<TextureComponent>());
	burgerManager->SetName(EnumStrings[BurgerManager]);

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


	burgerManager->GetComponent<TextureComponent>()->SetTexture(name2);
	burgerManager->GetComponent<TextureComponent>()->SetName(name);
	burgerManager->GetTransform()->AddTranslate(0, WindowBuffer);
	burgerManager->AddComponent(std::make_unique<BurgerManagerComponent>(scene));

	auto observer{ std::make_shared<StageClearedObserver>(&ReloadStageOfNr, &CreateEndScreen, scene) };

	auto burgerManagerComp{ burgerManager->GetComponent<BurgerManagerComponent>() };
	burgerManagerComp->AddObserver(observer);

	scene->Add(burgerManager);
	CreateObjects(scene, burgerManagerComp, burgerManager, isVersus, name);

	//FileReader* file{ new FileReader("../Data/galagamap.txt") };
	//auto str{ file->ReadGameDataFile() };
	//auto data{ file->ParseData(str, '+') };
	//auto stageItems = std::any_cast<std::map<std::string, std::vector<std::vector<int>>>>(data[name]);
	//std::vector<std::vector<int>> ladders = stageItems[EnumStrings[Ladders]];
	//std::vector<std::vector<int>> platforms = stageItems[EnumStrings[Platforms]];
	//std::vector<std::vector<int>> pattiesTop = stageItems[EnumStrings[PattyTop]];
	//std::vector<std::vector<int>> pattiesBottom = stageItems[EnumStrings[PattyBottom]];
	//std::vector<std::vector<int>> veggies = stageItems[EnumStrings[Veggie]];
	//std::vector<std::vector<int>> burgers = stageItems[EnumStrings[Burger]];
	//std::vector<std::vector<int>> plates = stageItems[EnumStrings[Plates]];
	////burger maneger has to detect when another burger piece overlaps with another, if yes they fall down until they detect a platform. 
	////If that platform has a burger piece, that piece has to fall down as well.
	//for (size_t i = 0; i < ladders.size(); i++)
	//{
	//	SDL_Rect rect2{ ladders[i][0]-16, ladders[i][1] + WindowBuffer,ladders[i][2],ladders[i][3] };

	//	auto ladder2 = std::make_shared<GameObject>();
	//	ladder2->AddComponent(std::make_unique<PlatformComponent>(rect2));
	//	burgerManagerComp->AddLadder(rect2, ladder2.get());
	//	ladder2->SetName(EnumStrings[Ladders]);
	//	scene->Add(ladder2);
	//}
	//for (size_t i = 0; i < platforms.size(); i++)
	//{
	//	SDL_Rect rect{ platforms[i][0], platforms[i][1] + WindowBuffer,platforms[i][2],platforms[i][3] };

	//	auto platform = std::make_shared<GameObject>();
	//	platform->AddComponent(std::make_unique<PlatformComponent>(rect));
	//	burgerManagerComp->AddPlatform(rect, platform.get());
	//	platform->SetName(EnumStrings[Platforms]);
	//	scene->Add(platform);
	//}
	//for (size_t i = 0; i < plates.size(); i++)
	//{
	//	auto plate = std::make_shared<GameObject>();
	//	SDL_Rect rect{ plates[i][0], plates[i][1] + WindowBuffer,plates[i][2],plates[i][3] };
	//	plate->AddComponent(std::make_unique<PlatformComponent>(rect));
	//	burgerManagerComp->AddPlate(rect, plate.get());
	//	plate->SetName(EnumStrings[Plates]);
	//	scene->Add(plate);
	//}

	//for (size_t i = 0; i < pattiesTop.size(); i++)
	//{
	//	GameObject* pattyTop = new GameObject();
	//	pattyTop->AddComponent(std::make_unique<TextureComponent>());
	//	pattyTop->GetComponent<TextureComponent>()->SetTexture("pattyTop.png");
	//	pattyTop->GetComponent<TextureComponent>()->Scale(3, 3);
	//	pattyTop->GetTransform()->Translate(static_cast<float>(pattiesTop[i][0]), static_cast<float>(pattiesTop[i][1])+ WindowBuffer);
	//	pattyTop->AddComponent(std::make_unique<BurgerComponent>(scene));	
	//	pattyTop->SetName(EnumStrings[PattyTop]);
	//	burgerManager->AddChild(pattyTop);

	//	GameObject* veggie = new GameObject();
	//	if (veggies.size() != 0) {
	//		veggie->AddComponent(std::make_unique<TextureComponent>());
	//		veggie->GetComponent<TextureComponent>()->SetTexture("veggie.png");
	//		veggie->GetComponent<TextureComponent>()->Scale(3, 3);
	//		veggie->GetTransform()->Translate(static_cast<float>(veggies[i][0]), static_cast<float>(veggies[i][1]) + WindowBuffer);
	//		veggie->AddComponent(std::make_unique<BurgerComponent>(scene));
	//		veggie->SetName(EnumStrings[Veggie]);
	//		burgerManager->AddChild(veggie);
	//	}
	//	else {
	//		delete veggie;
	//		veggie = nullptr;
	//	}

	//	GameObject* burger = new GameObject();
	//	burger->AddComponent(std::make_unique<TextureComponent>());
	//	burger->GetComponent<TextureComponent>()->SetTexture("burger.png");
	//	burger->GetComponent<TextureComponent>()->Scale(3, 3);
	//	burger->GetTransform()->Translate(static_cast<float>(burgers[i][0]), static_cast<float>(burgers[i][1]) + WindowBuffer);
	//	burger->AddComponent(std::make_unique<BurgerComponent>(scene));
	//	burger->SetName(EnumStrings[Burger]);
	//	burgerManager->AddChild(burger);

	//	GameObject* pattyBottom = new GameObject();
	//	pattyBottom->AddComponent(std::make_unique<TextureComponent>());
	//	pattyBottom->GetComponent<TextureComponent>()->SetTexture("pattyBottom.png");
	//	pattyBottom->GetComponent<TextureComponent>()->Scale(3, 3);
	//	pattyBottom->GetTransform()->Translate(static_cast<float>(pattiesBottom[i][0]), static_cast<float>(pattiesBottom[i][1]) + WindowBuffer);
	//	pattyBottom->AddComponent(std::make_unique<BurgerComponent>(scene));
	//	pattyBottom->SetName(EnumStrings[PattyBottom]);
	//	burgerManager->AddChild(pattyBottom);

	//	burgerManagerComp->AddBurger(pattyTop, pattyBottom, veggie, burger);
	//}
	//scene->Add(burgerManager);

	//if (!isVersus) {
	//	//enemies
	//	std::shared_ptr<GameObject> enemyHolder = std::make_shared<dae::GameObject>();
	//	enemyHolder->SetName(EnumStrings[EnemyHolder]);
	//	scene->Add(enemyHolder);

	//	std::vector<std::string> names{ "hotdog","egg", "pickle", "hotdog" };
	//	std::vector<EnemyType> types{ EnemyType::Hotdog, EnemyType::Egg, EnemyType::Pickle, EnemyType::Hotdog };
	//	for (size_t i = 0; i < 4; i++)
	//	{
	//		GameObject* enemy = new GameObject();
	//		enemy->SetName(EnumStrings[Enemy]);
	//		enemy->AddComponent(std::make_unique<EnemyComponent>(scene, types[i]));
	//		enemy->AddComponent(std::make_unique<TextureComponent>());
	//		enemy->GetComponent<TextureComponent>()->SetTexture(names[i] + "Down.png");
	//		enemy->GetComponent<TextureComponent>()->Scale(3, 3);
	//		enemy->GetComponent<TextureComponent>()->SetNrOfFrames(2);
	//		enemy->GetComponent<TextureComponent>()->GetRect();
	//		enemy->GetTransform()->Translate(Margin, (WindowSizeY)-Margin * 5);
	//		enemyHolder->AddChild(enemy);
	//	}
	//}

	scene->GetGameObject(EnumStrings[Global])->GetComponent<AudioComponent>()->PlayMenuSound(true);
}

void MakeMrHotdog(dae::Scene* scene, glm::vec2 startPos) {
	std::shared_ptr<GameObject> enemyHolder = std::make_shared<dae::GameObject>();
	enemyHolder->SetName(EnumStrings[EnemyHolder]);
	scene->Add(enemyHolder);

	std::shared_ptr<GameObject> opposer = std::make_shared<GameObject>();
	opposer->SetName(EnumStrings[Opposer]);

	opposer->AddComponent(std::make_unique<TextureComponent>());
	opposer->GetComponent<TextureComponent>()->SetTexture("hotdogDown.png");
	opposer->GetComponent<TextureComponent>()->Scale(3, 3);
	opposer->GetComponent<TextureComponent>()->SetNrOfFrames(2);
	opposer->GetComponent<TextureComponent>()->GetRect();
	opposer->GetComponent<TextureComponent>()->SetPosition((GameWindowSizeX) / 2 - Margin, WindowSizeY - Margin * 3);

	opposer->AddComponent(std::make_unique<PlayerComponent>(scene, true, startPos));
	opposer->GetComponent<PlayerComponent>()->SetIsController(true);
	opposer->GetTransform()->Translate(startPos);
	scene->Add(opposer);

	opposer->AddComponent(std::make_unique<MoveControllerComponent>(opposer->GetTransform()->GetPosition()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadUp, 1 }, std::make_unique<MoveController>(opposer.get(), PlayerComponent::Movement::UP, "hotdogUp.png", glm::vec3(0, -200.0f, 0.0f)));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadDown, 1 }, std::make_unique<MoveController>(opposer.get(), PlayerComponent::Movement::DOWN, "hotdogDown.png", glm::vec3(0, 200.0f, 0.0f)));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadLeft, 1 }, std::make_unique<MoveController>(opposer.get(), PlayerComponent::Movement::LEFT, "hotdogLeft.png", glm::vec3(-200.f, 0.0f, 0.0f)));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadRight, 1 }, std::make_unique<MoveController>(opposer.get(), PlayerComponent::Movement::RIGHT, "hotdogRight.png", glm::vec3(200.f, 0.0f, 0.0f)));

	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadUp, 1 }, std::make_unique<StopMoveController>(opposer.get()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadDown, 1 }, std::make_unique<StopMoveController>(opposer.get()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadLeft, 1 }, std::make_unique<StopMoveController>(opposer.get()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadRight, 1 }, std::make_unique<StopMoveController>(opposer.get()));
}

void MakePlayer(dae::Scene* scene, std::string textureName, int id, glm::vec2 startPos) {
	auto playerName{ EnumStrings[PlayerGeneral] + std::to_string(id) };

	//Main Player
	std::shared_ptr<GameObject> mainPlayer = std::make_shared<dae::GameObject>();
	scene->Add(mainPlayer);
	mainPlayer->SetName(playerName);

	//Texture
	mainPlayer->AddComponent(std::make_unique<TextureComponent>());
	mainPlayer->GetComponent<TextureComponent>()->SetName(playerName);
	mainPlayer->GetComponent<TextureComponent>()->SetTexture(textureName, 0.3f, 3);
	mainPlayer->GetComponent<TextureComponent>()->Scale(3, 3);
	mainPlayer->GetComponent<TextureComponent>()->GetRect();

	mainPlayer->AddComponent(std::make_unique<PlayerComponent>(scene, false, startPos));
	mainPlayer->GetTransform()->Translate(startPos);

	if (id == 0) {
		//Keyboard
		mainPlayer->GetComponent<PlayerComponent>()->SetIsController(false);
		mainPlayer->AddComponent(std::make_unique<MoveKeyboardComponent>(mainPlayer->GetTransform()->GetPosition()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_w, id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), PlayerComponent::Movement::UP, "moveUp.png", glm::vec3(0.f, -200.f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_s, id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), PlayerComponent::Movement::DOWN, "moveDown.png", glm::vec3(0.f, 200.f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_a, id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), PlayerComponent::Movement::LEFT, "moveLeft.png", glm::vec3(-200.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_d, id }, std::make_unique<MoveKeyboard>(mainPlayer.get(), PlayerComponent::Movement::RIGHT, "moveRight.png", glm::vec3(200.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED, SDLK_f, id }, std::make_unique<ThrowSalt>(mainPlayer.get()));

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
		mainPlayer->GetComponent<PlayerComponent>()->SetIsController(true);
		mainPlayer->AddComponent(std::make_unique<MoveControllerComponent>(mainPlayer->GetTransform()->GetPosition()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadUp, id }, std::make_unique<MoveController>(mainPlayer.get(), PlayerComponent::Movement::UP, "moveUpSalt.png", glm::vec3(0, -200.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadDown, id }, std::make_unique<MoveController>(mainPlayer.get(), PlayerComponent::Movement::DOWN, "moveDownSalt.png", glm::vec3(0, 200.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadLeft, id }, std::make_unique<MoveController>(mainPlayer.get(), PlayerComponent::Movement::LEFT, "moveLeftSalt.png", glm::vec3(-200.f, 0.0f, 0.0f)));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::DpadRight, id }, std::make_unique<MoveController>(mainPlayer.get(), PlayerComponent::Movement::RIGHT, "moveRightSalt.png", glm::vec3(200.f, 0.0f, 0.0f)));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_PRESSED,dae::ControllerButton::ButtonB, id }, std::make_unique<ThrowSalt>(mainPlayer.get()));

		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadUp, id }, std::make_unique<StopMoveController>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadDown, id }, std::make_unique<StopMoveController>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadLeft, id }, std::make_unique<StopMoveController>(mainPlayer.get()));
		Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP,dae::ControllerButton::DpadRight, id }, std::make_unique<StopMoveController>(mainPlayer.get()));
	}
	
	MakeValues(scene);
}

void MakeMrPepper(dae::Scene* scene, glm::vec2 startPos) {
	MakePlayer(scene, "moveUp.png", 0, startPos);
}

void MakeMrsSalt(dae::Scene* scene, glm::vec2 startPos) {
	MakePlayer(scene, "moveUpSalt.png", 1, startPos);
}

void MakeStage(dae::Scene* scene) {
	MakeStageOfNr(scene, Stages::Stage1, false);
}

void MakeVersusStage(dae::Scene* scene, glm::vec2 startPos) {
	//std::shared_ptr<GameObject> container = std::make_shared<dae::GameObject>();
	MakeStageOfNr(scene, Stages::Stage1, true);
	MakeMrHotdog(scene, startPos);
	//scene->Add(container);
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
	selector->AddComponent(std::make_unique<ModeSelector>(scene, &MakeMrPepper, &MakeMrsSalt, &MakeStage, &MakeVersusStage, &CreateScore));
	selector->AddComponent(std::make_unique<TextObjectComponent>(">", font));
	selector->GetComponent<TextObjectComponent>()->SetPosition(WindowSizeX / 2 - SubMargin * 2, 150);
	CreateSelectorInput(scene);

	auto go{ std::make_shared<GameObject>() };
	go->SetName(EnumStrings[Global]);
	go->AddComponent(std::make_unique<AudioComponent>());
	//go->AddComponent(new MoveKeyboardComponent(go->GetTransform()->GetPosition()));
	go->AddComponent(std::make_unique<MoveKeyboardComponent>(go->GetTransform()->GetPosition()));
	Input::GetInstance().BindKey({ ButtonStates::BUTTON_UP, SDLK_F1, 0 }, std::make_unique<Skip>(&MakeStageOfNr, &MakeMrPepper, scene));

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

		container->AddChild(scoreObj.get());
		container->AddChild(scoreObj2.get());

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