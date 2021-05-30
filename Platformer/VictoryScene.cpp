//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: MainMenuScene.cpp
// Description   	: A class for the Main menu, processed before the game is played.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//This include
#include "VictoryScene.h"

//Local includes
#include "Button.h"
#include "InputHandler.h"
#include "SceneManager.h"
#include "resource.h"
#include "ListBox.h"
#include "Iniparser.h"
#include "MainMenuScene.h"


//Library includes
#include <windows.h>

//Constructor
VictoryScene::VictoryScene(int playerWon)
	: Scene()
	, m_hasMouseClicked(false)
	, m_mousePos(0, 0), m_mousePressPos(0)
	, m_pBackBtn(0)
	, keypressed(-1)
{
	sceneID = MAINMENU;

	menuBackgroundTex.loadFromFile("Resources/images/Titles/Victory_Screen.jpg");
	menuBackgroundSpr.setTexture(menuBackgroundTex);

	switch (playerWon)
	{
	case 1:
		victorPlayerTex.loadFromFile("Resources/images/Titles/player1.jpg");
		break;
	case 2:
		victorPlayerTex.loadFromFile("Resources/images/Titles/player2.jpg");
		break;
	case 3:
		victorPlayerTex.loadFromFile("Resources/images/Titles/player3.jpg");
		break;
	case 4:
		victorPlayerTex.loadFromFile("Resources/images/Titles/player4.jpg");
		break;
	default:
		victorPlayerTex.loadFromFile("Resources/images/Titles/player1.jpg");
		break;
	}

	victorPlayerSpr.setTexture(victorPlayerTex);

	m_pBackBtn = new Button();

	//Quit button:
	sf::Sprite* temp = o_pRenderer->CreateSprite("images/Titles/back.jpg");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 5 / 7));
	m_pBackBtn->Initialise(temp, std::bind(&VictoryScene::Back, this));
}

//Destructor
VictoryScene::~VictoryScene()
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
void VictoryScene::Draw()
{
	o_pRenderer->Draw(menuBackgroundSpr);
	o_pRenderer->Draw(victorPlayerSpr);

	//Buttons:
	m_pBackBtn->Draw();

	//"Level Select"
	o_pRenderer->SetFontSize(20);
	o_pRenderer->SetColour(sf::Color::Black);
	o_pRenderer->DrawRectangle(355, 400, 152, 22);
	o_pRenderer->SetColour(sf::Color::White);

	o_pRenderer->SetFontSize(50);
	//o_pRenderer->DrawTextToView("Main Menu:"+ std::to_string(keypressed), 75, 100);

	o_pRenderer->SetColour(sf::Color::Black);
}

// Process( float dT )
//
// Description:	Calls all object's process function.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void VictoryScene::Update(float dT)
{
	keypressed = InputHandler::GetInstance().IsAnyKeyPressed();

	m_pBackBtn->Update();
}

void VictoryScene::Back()
{
	SceneManager::GetInstance().TransitionTo(new MainMenuScene());
}
