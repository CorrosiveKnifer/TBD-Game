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

//Library includes
#include <windows.h>

//Constructor
MainMenuScene::MainMenuScene()
	: Scene()
	, m_hasMouseClicked(false)
	, m_mousePos(0, 0), m_mousePressPos(0)
	, m_pPlayBtn(0), m_pSelectBtn(0), m_pSettingBtn(0), m_pQuitBtn(0)
	, keypressed(-1)
{	 
	m_pPlayBtn = new Button();
	m_pSelectBtn = new Button();
	m_pSettingBtn = new Button();
	m_pQuitBtn = new Button();

	//Play button:
	sf::Sprite* temp = o_pRenderer->CreateSprite("playButton.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(70, static_cast<float>(o_pRenderer->GetWindowSize().y * 3 / 7));
	m_pPlayBtn->Initialise(temp, std::bind(&MainMenuScene::Play, this));

	//Select button:
	temp = o_pRenderer->CreateSprite("selectButton.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(70, static_cast<float>(o_pRenderer->GetWindowSize().y * 4 / 7));
	m_pSelectBtn->Initialise(temp, std::bind(&MainMenuScene::Select, this));

	//Setting button:
	temp = o_pRenderer->CreateSprite("settingButton.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(70, static_cast<float>(o_pRenderer->GetWindowSize().y * 5 / 7));
	m_pSettingBtn->Initialise(temp, std::bind(&MainMenuScene::Settings, this));

	//Quit button:
	temp = o_pRenderer->CreateSprite("quitButton.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(70, static_cast<float>(o_pRenderer->GetWindowSize().y * 6 / 7));
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

	if (m_pPlayBtn != 0)
	{
		delete m_pPlayBtn;
		m_pPlayBtn = 0;
	}

	if (m_pSelectBtn != 0)
	{
		delete m_pSelectBtn;
		m_pSelectBtn = 0;
	}

	if (m_pSettingBtn != 0)
	{
		delete m_pSettingBtn;
		m_pSettingBtn = 0;
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
	//Buttons:
	m_pPlayBtn->Draw();
	m_pSelectBtn->Draw();
	m_pSettingBtn->Draw();
	m_pQuitBtn->Draw();

	//"Level Select"
	o_pRenderer->SetFontSize(20);
	o_pRenderer->SetColour(sf::Color::Black);
	o_pRenderer->DrawRectangle(355, 400, 152, 22);
	o_pRenderer->SetColour(sf::Color::White);

	o_pRenderer->SetFontSize(50);
	o_pRenderer->DrawTextToView("Main Menu:"+ std::to_string(keypressed), 75, 100);
	
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

	m_pPlayBtn->Update();
	m_pSelectBtn->Update();
	m_pSettingBtn->Update();
	m_pQuitBtn->Update();
}

void MainMenuScene::Play()
{

}

void MainMenuScene::Select()
{

}

void MainMenuScene::Settings()
{
	
}

void MainMenuScene::Quit()
{
	SceneManager::GetInstance().Quit();
}
