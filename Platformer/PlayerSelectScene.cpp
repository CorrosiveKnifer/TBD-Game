//This include
#include "PlayerSelectScene.h"

//Local includes
#include "Button.h"
#include "InputHandler.h"
#include "SceneManager.h"
#include "resource.h"
#include "ListBox.h"
#include "Iniparser.h"
#include "LogoScene.h"
#include <windows.h>
#include "Level_1.h"
#include "MainMenuScene.h"

//Library includes

//Constructor
PlayerSelectScene::PlayerSelectScene()
	: Scene()
	, m_hasMouseClicked(false)
	, m_mousePos(0, 0), m_mousePressPos(0)
	, m_pPlayBtn(0), m_pBackBtn(0)
	, keypressed(-1)
{

	m_pPlayBtn = new Button();
	m_pBackBtn = new Button();

	menuBackgroundTex.loadFromFile("Resources/images/Titles/Player_Select_Screen.jpg");
	menuBackgroundSpr.setTexture(menuBackgroundTex);

	//Player Select button:
	sf::Sprite* temp = o_pRenderer->CreateSprite("images/Titles/play_button.jpg");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x * 1.5 / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 6 / 7));
	m_pPlayBtn->Initialise(temp, std::bind(&PlayerSelectScene::Play, this));

	//Quit button:
	temp = o_pRenderer->CreateSprite("images/Titles/back.jpg");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x * 2 / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 6 / 7));
	m_pBackBtn->Initialise(temp, std::bind(&PlayerSelectScene::Back, this));
}

//Destructor
PlayerSelectScene::~PlayerSelectScene()
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
void PlayerSelectScene::Draw()
{

	o_pRenderer->Draw(menuBackgroundSpr);
	//Buttons:
	m_pPlayBtn->Draw();
	m_pBackBtn->Draw();

	//"Level Select"
	o_pRenderer->SetFontSize(20);
	o_pRenderer->SetColour(sf::Color::Black);
	o_pRenderer->DrawRectangle(355, 400, 152, 22);
	o_pRenderer->SetColour(sf::Color::White);

	o_pRenderer->SetFontSize(50);
	//o_pRenderer->DrawTextToView("Player Select:" + std::to_string(keypressed), 75, 100);

	o_pRenderer->SetColour(sf::Color::Black);
}

// Process( float dT )
//
// Description:	Calls all object's process function.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
void PlayerSelectScene::Update(float dT)
{
	keypressed = InputHandler::GetInstance().IsAnyKeyPressed();

	m_pPlayBtn->Update();
	m_pBackBtn->Update();
}

void PlayerSelectScene::Play()
{
	SceneManager::GetInstance().TransitionTo(new c_Level_1());
}

void PlayerSelectScene::Back()
{
	SceneManager::GetInstance().TransitionTo(new MainMenuScene());	
}
