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
#include "Level_2.h"
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

	sceneID = PLAYERSELECT;

	m_pPlayBtn = new Button();
	m_pBackBtn = new Button();

	menuBackgroundTex.loadFromFile("Resources/images/Titles/Player_Select_Screen.jpg");
	menuBackgroundSpr.setTexture(menuBackgroundTex);

	player1Tex.loadFromFile("Resources/images/Titles/player1.png");
	player1Spr.setTexture(player1Tex);
	player1Spr.setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x * 1 / 8) - (player1Spr.getGlobalBounds().width / 4), o_pRenderer->GetWindowSize().y * 2 / 4);
	player1Spr.setTextureRect(sf::IntRect(0, 0, player1Spr.getGlobalBounds().width / 2, player1Spr.getGlobalBounds().height));

	player2Tex.loadFromFile("Resources/images/Titles/player2.png");
	player2Spr.setTexture(player2Tex);
	player2Spr.setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x * 3 / 8) - (player2Spr.getGlobalBounds().width / 4), o_pRenderer->GetWindowSize().y * 2 / 4);
	player2Spr.setTextureRect(sf::IntRect(0, 0, player2Spr.getGlobalBounds().width / 2, player2Spr.getGlobalBounds().height));

	player3Tex.loadFromFile("Resources/images/Titles/player3.png");
	player3Spr.setTexture(player3Tex);
	player3Spr.setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x * 5 / 8) - (player3Spr.getGlobalBounds().width / 4), o_pRenderer->GetWindowSize().y * 2 / 4);
	player3Spr.setTextureRect(sf::IntRect(0, 0, player3Spr.getGlobalBounds().width / 2, player3Spr.getGlobalBounds().height));

	player4Tex.loadFromFile("Resources/images/Titles/player4.png");
	player4Spr.setTexture(player4Tex);
	player4Spr.setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x * 7 / 8) - (player4Spr.getGlobalBounds().width / 4), o_pRenderer->GetWindowSize().y * 2 / 4);
	player4Spr.setTextureRect(sf::IntRect(0, 0, player4Spr.getGlobalBounds().width / 2, player4Spr.getGlobalBounds().height));

	//Play button:
	sf::Sprite* temp = o_pRenderer->CreateSprite("images/Titles/play_button.jpg");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x * 1.5 / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 6 / 7));
	m_pPlayBtn->Initialise(temp, std::bind(&PlayerSelectScene::Play, this));

	//Back button:
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

	o_pRenderer->Draw(player1Spr);
	o_pRenderer->Draw(player2Spr);
	o_pRenderer->Draw(player3Spr);
	o_pRenderer->Draw(player4Spr);

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

	switch (InputHandler::GetInstance().playerJoystickIDs.size())
	{
	case 0:
		player1Spr.setTextureRect(sf::IntRect(0, 0, player1Spr.getGlobalBounds().width, player1Spr.getGlobalBounds().height));
		player2Spr.setTextureRect(sf::IntRect(0, 0, player2Spr.getGlobalBounds().width, player2Spr.getGlobalBounds().height));
		player3Spr.setTextureRect(sf::IntRect(0, 0, player3Spr.getGlobalBounds().width, player3Spr.getGlobalBounds().height));
		player4Spr.setTextureRect(sf::IntRect(0, 0, player4Spr.getGlobalBounds().width, player4Spr.getGlobalBounds().height));
		break;
	case 1:
		player1Spr.setTextureRect(sf::IntRect(player1Spr.getGlobalBounds().width, 0, player1Spr.getGlobalBounds().width, player1Spr.getGlobalBounds().height));
		player2Spr.setTextureRect(sf::IntRect(0, 0, player2Spr.getGlobalBounds().width, player2Spr.getGlobalBounds().height));
		player3Spr.setTextureRect(sf::IntRect(0, 0, player3Spr.getGlobalBounds().width, player3Spr.getGlobalBounds().height));
		player4Spr.setTextureRect(sf::IntRect(0, 0, player4Spr.getGlobalBounds().width, player4Spr.getGlobalBounds().height));
		break;
	case 2:
		player1Spr.setTextureRect(sf::IntRect(player1Spr.getGlobalBounds().width, 0, player1Spr.getGlobalBounds().width, player1Spr.getGlobalBounds().height));
		player2Spr.setTextureRect(sf::IntRect(player2Spr.getGlobalBounds().width, 0, player2Spr.getGlobalBounds().width, player2Spr.getGlobalBounds().height));
		player3Spr.setTextureRect(sf::IntRect(0, 0, player3Spr.getGlobalBounds().width, player3Spr.getGlobalBounds().height));
		player4Spr.setTextureRect(sf::IntRect(0, 0, player4Spr.getGlobalBounds().width, player4Spr.getGlobalBounds().height));
		break;
	case 3:
		player1Spr.setTextureRect(sf::IntRect(player1Spr.getGlobalBounds().width, 0, player1Spr.getGlobalBounds().width, player1Spr.getGlobalBounds().height));
		player2Spr.setTextureRect(sf::IntRect(player2Spr.getGlobalBounds().width, 0, player2Spr.getGlobalBounds().width, player2Spr.getGlobalBounds().height));
		player3Spr.setTextureRect(sf::IntRect(player3Spr.getGlobalBounds().width, 0, player3Spr.getGlobalBounds().width, player3Spr.getGlobalBounds().height));
		player4Spr.setTextureRect(sf::IntRect(0, 0, player4Spr.getGlobalBounds().width, player4Spr.getGlobalBounds().height));
		break;
	case 4:
		player1Spr.setTextureRect(sf::IntRect(player1Spr.getGlobalBounds().width, 0, player1Spr.getGlobalBounds().width, player1Spr.getGlobalBounds().height));
		player2Spr.setTextureRect(sf::IntRect(player2Spr.getGlobalBounds().width, 0, player2Spr.getGlobalBounds().width, player2Spr.getGlobalBounds().height));
		player3Spr.setTextureRect(sf::IntRect(player3Spr.getGlobalBounds().width, 0, player3Spr.getGlobalBounds().width, player3Spr.getGlobalBounds().height));
		player4Spr.setTextureRect(sf::IntRect(player4Spr.getGlobalBounds().width, 0, player4Spr.getGlobalBounds().width, player4Spr.getGlobalBounds().height));
		break;
	default:
		break;
	}

	if (SceneManager::GetInstance().m_topScene->sceneID == PLAYERSELECT)
	{
		switch (menuSelection)
		{
		case 1:
			m_pPlayBtn->m_value = 1.0f;
			m_pBackBtn->m_value = 0.0f;
			break;
		case 2:
			m_pPlayBtn->m_value = 0.0f;
			m_pBackBtn->m_value = 1.0f;
			break;
		default:
			break;
		}

		if (SceneManager::GetInstance().m_topScene->buttonPressed)
		{
			switch (menuSelection)
			{
			case 1:
				PlayerSelectScene::Play();
				break;
			case 2:
				PlayerSelectScene::Back();
				break;
			default:
				break;
			}
			SceneManager::GetInstance().m_topScene->buttonPressed = false;
		}

		if (SceneManager::GetInstance().m_topScene->leftPressed)
		{
			menuSelection--;
			if (menuSelection < 1)
			{
				menuSelection = 1;
			}
			if (menuSelection > 2)
			{
				menuSelection = 2;
			}
			SceneManager::GetInstance().m_topScene->leftPressed = false;
		}
		if (SceneManager::GetInstance().m_topScene->rightPressed)
		{
			menuSelection++;
			if (menuSelection < 1)
			{
				menuSelection = 1;
			}
			if (menuSelection > 2)
			{
				menuSelection = 2;
			}
			SceneManager::GetInstance().m_topScene->rightPressed = false;
		}
	}

	m_pPlayBtn->Update();
	m_pBackBtn->Update();
}

void PlayerSelectScene::Play()
{	
	if (InputHandler::GetInstance().playerJoystickIDs.size() > 1)
	{
		SceneManager::GetInstance().TransitionTo(new c_Level_1(InputHandler::GetInstance().playerJoystickIDs.size()));
	}
}

void PlayerSelectScene::Back()
{
	SceneManager::GetInstance().TransitionTo(new MainMenuScene());	
}
