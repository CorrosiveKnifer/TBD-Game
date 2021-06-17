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

	//VolumeUp button:
	sf::Sprite* temp = o_pRenderer->CreateSprite("images/Titles/up.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 3 / 7));
	m_pVolumeUpBtn->Initialise(temp, std::bind(&SettingsScene::volumeUp, this));

	//VolumeDown button:
	temp = o_pRenderer->CreateSprite("images/Titles/down.png");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 4 / 7));
	m_pVolumeDownBtn->Initialise(temp, std::bind(&SettingsScene::volumeDown, this));

	//Back button:
	temp = o_pRenderer->CreateSprite("images/Titles/back.jpg");
	temp->setScale(0.8f, 0.7f);
	temp->setPosition(static_cast<float>(o_pRenderer->GetWindowSize().x / 2) - (temp->getGlobalBounds().width / 4), static_cast<float>(o_pRenderer->GetWindowSize().y * 6 / 7));
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

	//Buttons:
	m_pVolumeUpBtn->Draw();
	m_pVolumeDownBtn->Draw();
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

void SettingsScene::Update(float dT)
{
	keypressed = InputHandler::GetInstance().IsAnyKeyPressed();

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
	std::cout << m_pVolume << std::endl;
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
	std::cout << m_pVolume << std::endl;
}

void SettingsScene::Back()
{
	SceneManager::GetInstance().TransitionTo(new MainMenuScene());
}
