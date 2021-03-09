//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	OverlayScene.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	OverlayScene
// Description:
//		Overlay Scene which uses another scene as a render (simulating a pause screen).
//

//Self Include
#include "OverlayScene.h"

//Local Includes
#include "SceneManager.h"
#include "InputHandler.h"
#include "TextGenerator.h"
#include "SettingsMenuScene.h"
#include "AudioManager.h"
#include "Button.h"

//Constructor
OverlayScene::OverlayScene(SceneADT* _belowScene, bool isPaused)
{
	m_isPaused = isPaused;

	m_belowScene = _belowScene;
	m_textGen60 = &Renderer::GetInstance().CreateTextGenerator("Avara.otf", 60);

	m_pResumeButton = new Button("Resume game", "ButtonBase.png");
	m_pResumeButton->SetLocalPosition(glm::vec2(Renderer::SCREEN_WIDTH * 0.20f, 480.0f));

	m_pSettingButton = new Button("Settings", "ButtonBase.png");
	m_pSettingButton->SetLocalPosition(glm::vec2(Renderer::SCREEN_WIDTH * 0.20f, 390.0f));

	m_pMenuButton = new Button("Quit to Main Menu", "ButtonBase.png");
	m_pMenuButton->SetLocalPosition(glm::vec2(Renderer::SCREEN_WIDTH * 0.20f, 300.0f));
	AudioManager::GetInstance().StopSoundEffects();
}

//Destuctor
OverlayScene::~OverlayScene()
{
	delete m_pResumeButton;
	m_pResumeButton = 0;

	delete m_pSettingButton;
	m_pSettingButton = 0;

	delete m_pMenuButton;
	m_pMenuButton = 0;

	m_belowScene = 0;
	m_textGen60 = 0;
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
void OverlayScene::Draw()
{
	m_belowScene->Draw();
	m_textGen60->RenderText("~ PAUSE ~", glm::vec2(Renderer::SCREEN_WIDTH * 0.30f, 600));
	//m_pResumeButton->Draw();
	//m_pSettingButton->Draw();
	//m_pMenuButton->Draw();
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
void OverlayScene::Process(float dT)
{
	if (!m_isPaused)
	{
		m_belowScene->Process(dT);
	}

	m_pResumeButton->Process(dT);
	m_pSettingButton->Process(dT);
	m_pMenuButton->Process(dT);
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
void OverlayScene::OnMousePositionChange()
{
	glm::vec2 mouse = InputHandler::GetInstance().GetMousePos();

	m_pResumeButton->HandleMouseHover(mouse);
	m_pSettingButton->HandleMouseHover(mouse);
	m_pMenuButton->HandleMouseHover(mouse);
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
void OverlayScene::OnMouseButtonChange()
{
	if (InputHandler::GetInstance().IsMousePressed(GLUT_LEFT_BUTTON))
	{
		glm::vec2 mouse = InputHandler::GetInstance().GetMousePos();
		if (m_pResumeButton->HandleMouseClick(mouse))
		{
			SceneManager::PopScene();
			return;
		}

		if (m_pSettingButton->HandleMouseClick(mouse))
		{
			SceneManager::PushScene(new SettingsMenuScene(!m_isPaused));
			return;
		}

		if (m_pMenuButton->HandleMouseClick(mouse))
		{
			SceneManager::PopScene(); //This
			SceneManager::PopScene(); //Game
			return;
		}
	}
}

//	OnKeyChange( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Called via the scene manager to alert that a keyboard key has changed states.
//		
//	Param:
//		- n/a	|	
//
//	Return: n/a		|
//
void OverlayScene::OnKeyChange()
{
	if (InputHandler::GetInstance().IsKeyPressed(27))
	{
		SceneManager::PopScene();
		return;
	}
}

//	OnSpecialChange( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Called via the scene manager to alert that a special key has changed states.
//		
//	Param:
//		- n/a	|	
//
//	Return: n/a		|
//
void OverlayScene::OnSpecialChange()
{

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
void OverlayScene::Resize()
{
	
}
