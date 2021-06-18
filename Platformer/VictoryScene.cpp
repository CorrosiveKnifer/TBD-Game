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
VictoryScene::VictoryScene()
	: Scene()
	, m_hasMouseClicked(false)
	, m_mousePos(0, 0), m_mousePressPos(0)
	, m_pBackBtn(0)
	, keypressed(-1)
{
	sceneID = MAINMENU;

	menuBackgroundTex.loadFromFile("Resources/images/Titles/Victory_Screen_Resized.jpg");
	menuBackgroundSpr.setTexture(menuBackgroundTex);
	o_pRenderer->LoadFont("fonts/ariblk.ttf");
	m_highscore = 0;
	int playerWon = 0;
	for (int i = 1; i <= 4; i++)
	{
		switch (i)
		{
		default:
		case 1:
			if (C_GlobalVariables::Player_1_Score > m_highscore)
			{
				m_highscore = C_GlobalVariables::Player_1_Score;
				playerWon = i;
			};
			break;
		case 2:
			if (C_GlobalVariables::Player_2_Score > m_highscore)
			{
				m_highscore = C_GlobalVariables::Player_2_Score;
				playerWon = i;
			};
			break;
		case 3:
			if (C_GlobalVariables::Player_3_Score > m_highscore)
			{
				m_highscore = C_GlobalVariables::Player_3_Score;
				playerWon = i;
			};
			break;
		case 4:
			if (C_GlobalVariables::Player_4_Score > m_highscore)
			{
				m_highscore = C_GlobalVariables::Player_4_Score;
				playerWon = i;
			};
			break;
		}
	}


	switch (playerWon)
	{
	case 1:
		victorPlayerTex.loadFromFile("Resources/images/Titles/player1.png");
		break;
	case 2:
		victorPlayerTex.loadFromFile("Resources/images/Titles/player2.png");
		break;
	case 3:
		victorPlayerTex.loadFromFile("Resources/images/Titles/player3.png");
		break;
	case 4:
		victorPlayerTex.loadFromFile("Resources/images/Titles/player4.png");
		break;
	default:
		victorPlayerTex.loadFromFile("Resources/images/Titles/player1.png");
		break;
	}


	victorPlayerSpr.setTexture(victorPlayerTex);
	victorPlayerSpr.setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (victorPlayerSpr.getGlobalBounds().width / 4), o_pRenderer->GetWindowSize().y / 2);
	victorPlayerSpr.setTextureRect(sf::IntRect(victorPlayerSpr.getGlobalBounds().width / 2, 0, victorPlayerSpr.getGlobalBounds().width, victorPlayerSpr.getGlobalBounds().height));

	m_pBackBtn = new Button();

	//Quit button:
	sf::Sprite* temp = o_pRenderer->CreateSprite("images/Titles/back.jpg");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 6 / 7));
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
	//o_pRenderer->SetFontSize(20);
	//o_pRenderer->SetColour(sf::Color::Black);
	//o_pRenderer->DrawRectangle(355, 400, 152, 22);
	//o_pRenderer->SetColour(sf::Color::White);
	//
	//o_pRenderer->SetFontSize(50);
	//o_pRenderer->DrawTextToView("Main Menu:"+ std::to_string(keypressed), 75, 100);
	o_pRenderer->SetColour(sf::Color::White);
	o_pRenderer->SetFontAlign(Align::Centre);
	o_pRenderer->DrawTextToWorld("Score: " + std::to_string((int)m_highscore), C_GlobalVariables::ScreenSizeX * 0.5, C_GlobalVariables::ScreenSizeY * 0.80);
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
