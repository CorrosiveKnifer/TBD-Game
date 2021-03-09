//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	GameOverScene.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	GameOverScene
// Description:
//		Final Scene which will show once the game is over, indicating a loss or win.
//

//Self Include
#include "GameOverScene.h"

//Local Includes
#include "SceneManager.h"
#include "InputHandler.h"
#include "TextGenerator.h"
#include "AudioManager.h"
#include "Button.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Program.h"

//Constructor
GameOverScene::GameOverScene(bool _isWin, unsigned int kills, double time, float accuracy)
{
	m_isWin = _isWin;
	m_kills = kills;
	m_gameTime = time;
	m_accuracy = accuracy;
}

//Destructor
GameOverScene::~GameOverScene()
{
	delete m_pBackButton;
	m_pBackButton = 0;

	delete m_pCubeMap;
	m_pCubeMap = 0;

	delete m_pMainCam;
	m_pMainCam = 0;

	m_textGen60 = 0;

	AudioManager::GetInstance().StopMusic();
}

bool GameOverScene::Initialise()
{
	m_program = &Renderer::GetInstance().CreateDefaultProgram();
	m_textGen60 = &Renderer::GetInstance().CreateTextGenerator("Technical.ttf", 60);
	m_pBackButton = new Button("Back to main menu", "ButtonBase.png");
	m_pBackButton->SetLocalScale(glm::vec2(0.5f, 0.8f));

	std::string files[6];
	for (unsigned int i = 0; i < 6; i++)
		files[i] = "space_c0" + std::to_string(i) + ".png";

	m_pMainCam = new Camera(Camera::CamType::Perspective);
	m_pCubeMap = new CubeMap(*m_pMainCam, Renderer::GetInstance().CreateCubeMapTexture("SpaceCubeMap", files));

	m_timeElapsed = 0.0f;

	AudioManager::GetInstance().StopSoundEffects();

	if (m_isWin)
	{
		AudioManager::GetInstance().CreateSound("GameWin.wav", FMOD_LOOP_NORMAL);
		AudioManager::GetInstance().PlayMusic("GameWin.wav");
	}
	else
	{
		AudioManager::GetInstance().CreateSound("GameOver.wav", FMOD_LOOP_NORMAL);
		AudioManager::GetInstance().PlayMusic("GameOver.wav");
	}

	return true;
}

void GameOverScene::PreLoad()
{

}

//	Draw( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Draws all entities and objects within the scene.
//		
//	Param:
//		- n/a	|	
//
//	Return: n/a		|
//
void GameOverScene::Draw()
{
	m_pCubeMap->Draw(m_pMainCam, nullptr);

	m_textGen60->SetColour(WhiteText);
	if (m_isWin)
	{
		float offset = m_textGen60->GetDimentionsOfText("Game Win").x * 0.5f;
		m_textGen60->RenderText("Game Win", glm::vec2(Renderer::SCREEN_WIDTH * 0.50f - offset, 600));
	}
	else
	{
		float offset = m_textGen60->GetDimentionsOfText("Game Over").x * 0.5f;
		m_textGen60->RenderText("Game Over", glm::vec2(Renderer::SCREEN_WIDTH * 0.50f - offset, 600));
	}
	m_textGen60->RenderText("Total Kills: "+std::to_string(m_kills), glm::vec2(Renderer::SCREEN_WIDTH * 0.10f, 450));

	int minutes = static_cast<int>(floor(m_gameTime / 60.0f));
	int seconds = (int)(m_gameTime) % 60;
	m_textGen60->RenderText("Total Time: ", glm::vec2(Renderer::SCREEN_WIDTH * 0.10f, 350));
	m_textGen60->RenderText(std::to_string(minutes) + "mins " + std::to_string(seconds) + "sec", glm::vec2(Renderer::SCREEN_WIDTH * 0.25f, 290));

	int percent = (int)(m_accuracy * 100.0f);
	m_textGen60->RenderText("Hit Percentage: "+std::to_string(percent)+"%", glm::vec2(Renderer::SCREEN_WIDTH * 0.10f, 200));

	//m_pBackButton->Draw();
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes all entities and objects within the scene since the last call.
//		
//	Param:
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void GameOverScene::Process(float dT)
{
	m_timeElapsed += dT / 6.0f;

	//Rotate camera
	glm::vec3 pos = m_pMainCam->GetWorldPosition();
	pos.x = sin(m_timeElapsed) * 3.0f;
	pos.y = 1.5f;
	pos.z = cos(m_timeElapsed) * 3.0f;
	m_pMainCam->Process(dT);
	m_pMainCam->SetPosition(pos);

	m_pCubeMap->Process(dT);
	m_pBackButton->Process(dT);
}

//	OnMousePositionChange( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Called via the scene manager to alert that the mouse has moved.
//		
//	Param:
//		- n/a	|	
//
//	Return: n/a		|
//
void GameOverScene::OnMousePositionChange()
{
	glm::vec2 mouse = InputHandler::GetInstance().GetMousePos();
	m_pBackButton->HandleMouseHover(mouse);
}

//	OnMouseButtonChange( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Called via the scene manager to alert that a mouse button has changed states.
//		
//	Param:
//		- n/a	|	
//
//	Return: n/a		|
//
void GameOverScene::OnMouseButtonChange()
{
	if (InputHandler::GetInstance().IsMousePressed(GLUT_LEFT_BUTTON))
	{
		glm::vec2 mouse = InputHandler::GetInstance().GetMousePos();
		if (m_pBackButton->HandleMouseClick(mouse))
		{
			SceneManager::PopScene();
			//dynamic_cast<MainMenuScene*>(SceneManager::TopScene())->SetTimeElapsed(time);
			return;
		}
	}
}

//	Resize( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Called via the scene manager to alert that the window has resized.
//		
//	Param:
//		- n/a	|	
//
//	Return: n/a		|
//
void GameOverScene::Resize()
{
	m_pMainCam->RemakeMatrix();
}
