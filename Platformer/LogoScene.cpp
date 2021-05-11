//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: LogoScene.cpp
// Description   	: Used to display a single logo which fades in and out.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "LogoScene.h"

//Library Includes

//Local Includes
#include "InputHandler.h"
#include "SceneManager.h"
#include "MainMenuScene.h"
#include "Level_1.h"

//Constructor
LogoScene::LogoScene()
	: Scene()
	, m_isInputEnabled(true)
	, m_screenColor(sf::Color::Black)
{
	m_logo = o_pRenderer->CreateSprite("MDS.png");
	m_logo->setOrigin(sf::Vector2f(m_logo->getTextureRect().width / 2, m_logo->getTextureRect().height / 2));
	m_logo->setPosition(sf::Vector2f(o_pRenderer->GetWindowSize().x / 2.0f, o_pRenderer->GetWindowSize().y / 2.0f));

	m_renderTime = 2.5f;
	m_timePassed = 0.0f;
	m_hasFinished = false;
}

//Destructor
LogoScene::~LogoScene()
{
	if (m_logo != 0)
	{
		delete m_logo;
		m_logo = 0;
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
void LogoScene::Draw()
{
	if(m_logo != 0)
		o_pRenderer->Draw(*m_logo);

	o_pRenderer->SetColour(m_screenColor);
}

// Process( float dT )
//
// Description:	Calls all object's process function.
//				
// @param	float	Change in time since the last process call.
//
// @return	n/a
//
void LogoScene::Update(float dt)
{
	m_timePassed += dt;

	if (m_logo != 0)
	{
		if (m_timePassed < m_renderTime / 2) //Fade in
		{
			m_logo->setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * (m_timePassed / (m_renderTime / 2)))));
		}
		else if (m_timePassed > m_renderTime / 2) //Fade out
		{
			float alpha = 1 - (m_timePassed - (m_renderTime / 2)) / (m_renderTime / 2);
			m_logo->setColor(sf::Color(255, 255, 255, static_cast<sf::Uint8>(255 * alpha)));
		}
		else if (m_timePassed > m_renderTime) //Reset colour
		{
			m_logo->setColor(sf::Color(255, 255, 255, 0));
		}
		else
		{
			m_logo->setColor(sf::Color(255, 255, 255, 255));
		}
	}

	if (m_timePassed >= m_renderTime)
	{
		if (m_hasFinished)
		{
			SceneManager::GetInstance().TransitionTo(new c_Level_1());
			return;
		}
			
		m_timePassed = 0.0f;
		m_logo->setTexture(*o_pRenderer->CreateTexture("Logo2D.png"), true);
		m_logo->setScale(0.8f, 0.8f);
		m_logo->setOrigin(sf::Vector2f(m_logo->getTextureRect().width/2, m_logo->getTextureRect().height/2));
		m_hasFinished = true;
	}
}

// SetLogo( sf::Sprite* sp )
//
// Description:	Updates the logo to a new one, while deleting the old one.
//				
// @param	sf::Sprite*		Pointer to the new logo.
//
// @return	n/a
//
void LogoScene::SetLogo(sf::Sprite* sp)
{
	if (m_logo != 0)
	{
		delete m_logo;
	}
	m_logo = sp;
	sf::Vector2f pos;
	pos.x = (o_pRenderer->GetWindowSize().x - (sp->getTexture()->getSize().x * sp->getScale().x)) / 2;
	pos.y = (o_pRenderer->GetWindowSize().y - (sp->getTexture()->getSize().y * sp->getScale().y)) / 2;
	m_logo->setPosition(pos);
}

// SetBackgroundColour( sf::Color col )
//
// Description:	Updates the background colour of the backbuffer.
//				
// @param	sf::Color	Background colour value.
//
// @return	n/a
//
void LogoScene::SetBackgroundColour(sf::Color col)
{
	m_screenColor = col;
}

// SetTimeLength( float _time )
//
// Description:	Updates the total length of time for the window to live for.
//				
// @param	float	Total time for the logo to be displayed.
//
// @return	n/a
//
//void LogoScene::SetTimeLength(float _time)
//{
//	m_renderTime = _time;
//}

// SetInputEnabled( bool )
//
// Description:	Updates if the user can send input during the logo screen.
//				
// @param	bool	if input is allowed
//
// @return	n/a
//
void LogoScene::SetInputEnabled(bool _input)
{
	m_isInputEnabled = _input;
}

// SetLogoLoc( sf::Vector2f )
//
// Description:	Sets the location of the logo on the screen.
//				
// @param	sf::Vector2f	new Logo position. 
//
// @return	n/a
//
void LogoScene::SetLogoLoc(sf::Vector2f logoLoc)
{
	if (m_logo != 0)
	{
		m_logo->setPosition(logoLoc);
	}
}