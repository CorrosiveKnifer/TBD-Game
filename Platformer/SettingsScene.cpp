#include "SettingsScene.h"

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

SettingsScene::SettingsScene()
	: Scene()
	, m_hasMouseClicked(false)
	, m_mousePos(0, 0), m_mousePressPos(0)
	, m_pVolumeUpBtn(0), m_pVolumeDownBtn(0), m_pBackBtn(0)
	, keypressed(-1)
{
	sceneID = SETTINGS;

	m_pVolumeUpBtn = new Button();
	m_pVolumeDownBtn = new Button();
	m_pBackBtn = new Button();

	menuBackgroundTex.loadFromFile("Resources/images/Titles/Menu_Screen_blank.jpg");
	menuBackgroundSpr.setTexture(menuBackgroundTex);

	masterVolumeTex.loadFromFile("Resources/images/Titles/music_volume.png");
	masterVolumeSpr.setTexture(masterVolumeTex);
	masterVolumeSpr.setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x * 2 / 8) - (masterVolumeSpr.getGlobalBounds().width / 4), o_pRenderer->GetWindowSize().y * 2 / 4);

	//VolumeUp button:
	sf::Sprite* temp = o_pRenderer->CreateSprite("images/Titles/up.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 3 / 8));
	m_pVolumeUpBtn->Initialise(temp, std::bind(&SettingsScene::volumeUp, this));

	//VolumeDown button:
	temp = o_pRenderer->CreateSprite("images/Titles/down.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 5 / 8));
	m_pVolumeDownBtn->Initialise(temp, std::bind(&SettingsScene::volumeDown, this));

	//Back button:
	temp = o_pRenderer->CreateSprite("images/Titles/back.jpg");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 6 / 8));
	m_pBackBtn->Initialise(temp, std::bind(&SettingsScene::Back, this));
}

SettingsScene::~SettingsScene()
{
	if (m_mousePressPos != 0)
	{
		delete m_mousePressPos;
		m_mousePressPos = 0;
	}
	if (m_pVolumeUpBtn != 0)
	{
		delete m_pVolumeUpBtn;
		m_pVolumeUpBtn = 0;
	}

	if (m_pVolumeDownBtn != 0)
	{
		delete m_pVolumeDownBtn;
		m_pVolumeDownBtn = 0;
	}

	if (m_pBackBtn != 0)
	{
		delete m_pBackBtn;
		m_pBackBtn = 0;
	}
}

void SettingsScene::Draw()
{
	o_pRenderer->Draw(menuBackgroundSpr);

	o_pRenderer->Draw(masterVolumeSpr);

	//Buttons:
	m_pVolumeUpBtn->Draw();
	m_pVolumeDownBtn->Draw();
	m_pBackBtn->Draw();

	//"Level Select"
	o_pRenderer->SetFontSize(20);
	o_pRenderer->SetColour(sf::Color::Black);
	o_pRenderer->DrawRectangle(150, 150, 150, 150);
	o_pRenderer->SetColour(sf::Color::White);

	o_pRenderer->SetFontSize(50);
	o_pRenderer->DrawTextToView(std::to_string((int)m_pVolume / 10), static_cast<float>(o_pRenderer->GetWindowSize().x * 1.95f / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 4.25 / 8));

	o_pRenderer->SetColour(sf::Color::Black);
}

void SettingsScene::Update(float dT)
{
	keypressed = InputHandler::GetInstance().IsAnyKeyPressed();

	if (SceneManager::GetInstance().m_topScene->sceneID == SETTINGS)
	{
		switch (menuSelection)
		{
		case 1:
			m_pVolumeUpBtn->m_value = 1.0f;
			m_pVolumeDownBtn->m_value = 0.0f;
			m_pBackBtn->m_value = 0.0f;
			break;
		case 2:
			m_pVolumeUpBtn->m_value = 0.0f;
			m_pVolumeDownBtn->m_value = 1.0f;
			m_pBackBtn->m_value = 0.0f;
			break;
		case 3:
			m_pVolumeUpBtn->m_value = 0.0f;
			m_pVolumeDownBtn->m_value = 0.0f;
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
				SettingsScene::volumeUp();
				break;
			case 2:
				SettingsScene::volumeDown();
				break;
			case 3:
				SettingsScene::Back();
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
			if (menuSelection > 3)
			{
				menuSelection = 3;
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
			if (menuSelection > 3)
			{
				menuSelection = 3;
			}
			SceneManager::GetInstance().m_topScene->downPressed = false;
		}
	}

	m_pVolumeUpBtn->Update();
	m_pVolumeDownBtn->Update();
	m_pBackBtn->Update();
}

void SettingsScene::volumeUp()
{
	if (m_pVolume != 100.0f && m_pVolume < 100.0f)
	{
		m_pVolume += 10.0f;
	}
	if (m_pVolume > 100.0f)
	{
		m_pVolume = 100.0f;
	}
}

void SettingsScene::volumeDown()
{
	if (m_pVolume != 0.0f && m_pVolume > 0.0f)
	{
		m_pVolume -= 10.0f;
	}
	if (m_pVolume < 0.0f)
	{
		m_pVolume = 0.0f;
	}
}

void SettingsScene::Back()
{
	SceneManager::GetInstance().TransitionTo(new MainMenuScene());
}
