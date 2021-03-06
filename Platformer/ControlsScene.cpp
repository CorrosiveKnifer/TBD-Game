//This include
#include "ControlsScene.h"

//Local includes
#include "Button.h"
#include "InputHandler.h"
#include "SceneManager.h"
#include "resource.h"
#include "ListBox.h"
#include "Iniparser.h"
#include "LogoScene.h"
#include "MainMenuScene.h"

//Library includes
#include <windows.h>

//Constructor
ControlsScene::ControlsScene()
	: Scene()
	, m_hasMouseClicked(false)
	, m_mousePos(0, 0), m_mousePressPos(0)
	, m_pBackBtn(0)
	, keypressed(-1)
{
	sceneID = CONTROLS;

	m_pBackBtn = new Button();

	menuBackgroundTex.loadFromFile("Resources/images/Titles/Controls_Screen.jpg");
	menuBackgroundSpr.setTexture(menuBackgroundTex);

	//Back button:
	sf::Sprite* temp = o_pRenderer->CreateSprite("images/Titles/back.jpg");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 6 / 7));
	m_pBackBtn->Initialise(temp, std::bind(&ControlsScene::Back, this));
}

//Destructor
ControlsScene::~ControlsScene()
{
	if (m_mousePressPos != 0)
	{
		delete m_mousePressPos;
		m_mousePressPos = 0;
	}

	if (m_pBackBtn != 0)
	{
		delete m_pBackBtn;
		m_pBackBtn = 0;
	}
}

// DrawContents( BackBuffer& buffer )
//
// Description:	Draws all objects to the screen using the backbuffer provided.
//				
// @param	BackBuffer&		The back buffer to draw all of the objects to.
//
// @return	n/a
//
void ControlsScene::Draw()
{

	o_pRenderer->Draw(menuBackgroundSpr);

	//Buttons:
	m_pBackBtn->Draw();

	//"Level Select"
	o_pRenderer->SetFontSize(20);
	o_pRenderer->SetColour(sf::Color::Black);
	o_pRenderer->DrawRectangle(355, 400, 152, 22);
	o_pRenderer->SetColour(sf::Color::White);

	o_pRenderer->SetFontSize(50);
	//o_pRenderer->DrawTextToView("Controls:" + std::to_string(keypressed), 75, 100);

	o_pRenderer->SetColour(sf::Color::Black);
}

// Process( float dT )
//
// Description:	Calls all object's process function.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void ControlsScene::Update(float dT)
{
	keypressed = InputHandler::GetInstance().IsAnyKeyPressed();

	m_pBackBtn->m_value = 1.0f;
	if (SceneManager::GetInstance().m_topScene->sceneID == CONTROLS)
	{
		if (SceneManager::GetInstance().m_topScene->buttonPressed)
		{
			ControlsScene::Back();
			SceneManager::GetInstance().m_topScene->buttonPressed = false;
		}
	}
	
	m_pBackBtn->Update();
}

void ControlsScene::Back()
{
	SceneManager::GetInstance().TransitionTo(new MainMenuScene());
}
