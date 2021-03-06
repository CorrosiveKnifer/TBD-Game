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
#include "MainMenuScene.h"

//Local includes
#include "Button.h"
#include "InputHandler.h"
#include "SceneManager.h"
#include "resource.h"
#include "ListBox.h"
#include "Iniparser.h"
#include "LogoScene.h"
#include "ControlsScene.h"
#include "PlayerSelectScene.h"
#include "SettingsScene.h"
#include "VictoryScene.h"
//Library includes
#include <windows.h>

//Constructor
MainMenuScene::MainMenuScene()
	: Scene()
	, m_hasMouseClicked(false)
	, m_mousePos(0, 0), m_mousePressPos(0)
	, m_pPlayerSelectBtn(0), m_pControlsBtn(0), m_pSettingsBtn(0), m_pQuitBtn(0)
	, keypressed(-1)
{	 
	sceneID = MAINMENU;

	menuBackgroundTex.loadFromFile("Resources/images/Titles/Menu_Screen_blank.jpg");
	menuBackgroundSpr.setTexture(menuBackgroundTex);
	
	m_pPlayerSelectBtn = new Button();
	m_pControlsBtn = new Button();
	m_pSettingsBtn = new Button();
	m_pQuitBtn = new Button();

	//Player Select button:
	sf::Sprite* temp = o_pRenderer->CreateSprite("images/Titles/PlayerSelect.jpg");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 3 / 7));
	m_pPlayerSelectBtn->Initialise(temp, std::bind(&MainMenuScene::PlayerSelect, this));

	//Controls button:
	temp = o_pRenderer->CreateSprite("images/Titles/controls.jpg");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 4 / 7));
	m_pControlsBtn->Initialise(temp, std::bind(&MainMenuScene::Controls, this));

	//Settings button:
	temp = o_pRenderer->CreateSprite("images/Titles/settings.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 5 / 7));
	m_pSettingsBtn->Initialise(temp, std::bind(&MainMenuScene::Settings, this));

	//Quit button:
	temp = o_pRenderer->CreateSprite("images/Titles/quit.jpg");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 6 / 7));
	m_pQuitBtn->Initialise(temp, std::bind(&MainMenuScene::Quit, this));
}	

//Destructor
MainMenuScene::~MainMenuScene()
{
	if (m_mousePressPos != 0)
	{
		delete m_mousePressPos;
		m_mousePressPos = 0;
	}

	if (m_pPlayerSelectBtn != 0)
	{
		delete m_pPlayerSelectBtn;
		m_pPlayerSelectBtn = 0;
	}

	if (m_pControlsBtn != 0)
	{
		delete m_pControlsBtn;
		m_pControlsBtn = 0;
	}

	if (m_pSettingsBtn  != 0)
	{
		delete m_pSettingsBtn;
		m_pSettingsBtn = 0;
	}

	if (m_pQuitBtn != 0)
	{
		delete m_pQuitBtn;
		m_pQuitBtn = 0;
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
void MainMenuScene::Draw()
{
	o_pRenderer->Draw(menuBackgroundSpr);

	//Buttons:
	m_pPlayerSelectBtn->Draw();
	m_pControlsBtn->Draw();
	m_pSettingsBtn->Draw();
	m_pQuitBtn->Draw();

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
void MainMenuScene::Update(float dT)
{
	keypressed = InputHandler::GetInstance().IsAnyKeyPressed();

	if (SceneManager::GetInstance().m_topScene->sceneID == MAINMENU)
	{
		switch (menuSelection)
		{
		case 1:
			m_pPlayerSelectBtn->m_value = 1.0f;
			m_pControlsBtn->m_value = 0.0f;
			m_pSettingsBtn->m_value = 0.0f;
			m_pQuitBtn->m_value = 0.0f;
			break;
		case 2:
			m_pPlayerSelectBtn->m_value = 0.0f;
			m_pControlsBtn->m_value = 1.0f;
			m_pSettingsBtn->m_value = 0.0f;
			m_pQuitBtn->m_value = 0.0f;
			break;
		case 3:
			m_pPlayerSelectBtn->m_value = 0.0f;
			m_pControlsBtn->m_value = 0.0f;
			m_pSettingsBtn->m_value = 1.0f;
			m_pQuitBtn->m_value = 0.0f;
			break;
		case 4:
			m_pPlayerSelectBtn->m_value = 0.0f;
			m_pControlsBtn->m_value = 0.0f;
			m_pSettingsBtn->m_value = 0.0f;
			m_pQuitBtn->m_value = 1.0f;
			break;
		default:
			break;
		}

		if (SceneManager::GetInstance().m_topScene->buttonPressed)
		{
			switch (menuSelection)
			{
			case 1:
				MainMenuScene::PlayerSelect();
				break;
			case 2:
				MainMenuScene::Controls();
				break;
			case 3:
				MainMenuScene::Settings();
				break;
			case 4:
				MainMenuScene::Quit();
				break;
			default:
				break;
			}
			SceneManager::GetInstance().m_topScene->buttonPressed = false;
		}

		if (SceneManager::GetInstance().m_topScene->upPressed)
		{
			menuSelection--;
			if (menuSelection < 1)
			{
				menuSelection = 1;
			}
			if (menuSelection > 4)
			{
				menuSelection = 4;
			}
			SceneManager::GetInstance().m_topScene->upPressed = false;
		}
		if (SceneManager::GetInstance().m_topScene->downPressed)
		{
			menuSelection++;
			if (menuSelection < 1)
			{
				menuSelection = 1;
			}
			if (menuSelection > 4)
			{
				menuSelection = 4;
			}
			SceneManager::GetInstance().m_topScene->downPressed = false;
		}
	}


	m_pPlayerSelectBtn->Update();
	m_pControlsBtn->Update();
	m_pSettingsBtn->Update();
	m_pQuitBtn->Update();
}

void MainMenuScene::PlayerSelect()
{
	SceneManager::GetInstance().TransitionTo(new PlayerSelectScene());
}

void MainMenuScene::Controls()
{
	SceneManager::GetInstance().TransitionTo(new ControlsScene());
}

void MainMenuScene::Settings()
{
	SceneManager::GetInstance().TransitionTo(new SettingsScene());
}

void MainMenuScene::Quit()
{
	SceneManager::GetInstance().Quit();
}
