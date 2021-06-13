#include "PauseMenuScene.h"
#include "SceneManager.h"
#include "MainMenuScene.h"

PauseMenuScene::PauseMenuScene(Scene* underScene) : Scene()
{
	m_underScene = underScene;
}

PauseMenuScene::~PauseMenuScene()
{
	delete m_underScene;
	m_underScene = nullptr;
}

void PauseMenuScene::Draw()
{
	if(m_underScene != nullptr)
		m_underScene->Draw();

	o_pRenderer->SetColour(sf::Color(0, 0, 0, 100.0f));
	o_pRenderer->DrawRectangle(0, 0, 1920, 1080);
}

void PauseMenuScene::Update(float dt)
{
	for (unsigned int i = 0; i < 4; i++)
	{
		if(sf::Joystick::isButtonPressed(i, InputHandler::GetInstance().BUTTON_START))
		{
			SceneManager::GetInstance().UnPauseScene();
		}
	}
}

void PauseMenuScene::BackToMainMenu()
{
	delete m_underScene;
	m_underScene = new MainMenuScene();
	SceneManager::GetInstance().UnPauseScene();
}
	
