#include "ValuesComponent.h"
#include "Subject.h"
#include "FileReader.h"
#include "Event.h"

dae::ValuesComponent::~ValuesComponent()
{}

void dae::ValuesComponent::Update()
{}

void dae::ValuesComponent::FixedUpdate()
{}

void dae::ValuesComponent::Render() const
{}

void dae::ValuesComponent::Damage()
{
	m_Lives--;
	Event live{ EventType::Live };
	Notify(GetGameObject(), live);
	if (m_Lives <= 0) {
		Event gameOver{ EventType::GameOver };
		Notify(GetGameObject(), gameOver);
	}
}

void dae::ValuesComponent::IncreaseScore(int score)
{
	m_Score += score;
	auto go{ GetGameObject() };
	Event scoreEvent{ EventType::Score };
	Notify(go, scoreEvent);
}

int dae::ValuesComponent::GetLives() const
{
	return m_Lives;
}

void dae::ValuesComponent::SetLives(int lives)
{
	m_Lives = lives;
}

int dae::ValuesComponent::GetScores() const
{
	return m_Score;
}

void dae::ValuesComponent::ResetObserver()
{
	Event reset{ EventType::Reset };
	Notify(GetGameObject(), reset);
}

void dae::ValuesComponent::Reset()
{

	//m_Lives = 3;
	//m_Score = 0;
	//NrOfHits = 0;
	//NrOfShotsFired = 0;
}

void dae::ValuesComponent::GameEnd()
{
	m_Lives = 3;
	m_Score = 0;
	NrOfHits = 0;
	NrOfShotsFired = 0;
	//FileReader* file{ new FileReader("../Data/highscore.txt") };
	//auto str{ file->ReadGameDataFile() };
	//auto data{ file->ParseDataSimple(str, '+') };
	//auto score{ data["Highscore"] };
	//std::string scoreStr{ std::any_cast<std::string>(score) };
	//if (GetScores() > std::stoi(scoreStr)) {
	//	file->WriteData("Highscore", std::to_string(GetScores()));
	//}
}

int dae::ValuesComponent::GetMissRatio()
{
	if (NrOfHits == 0 || NrOfShotsFired == 0) return 0;
	auto ratio{ (static_cast<float>(NrOfHits) / static_cast<float>(NrOfShotsFired)) * 100.f };
	return static_cast<int>(ratio);
}
