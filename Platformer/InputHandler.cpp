//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: InputHandler.cpp
// Description   	: Handles the input from the keyboard and mouse on the window.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Self Include
#include "InputHandler.h"

//Local Includes
#include "SceneManager.h"
#include "Renderer.h"

//Static variables
InputHandler* InputHandler::sm_pInstance = nullptr;

InputHandler& InputHandler::GetInstance()
{
	if (sm_pInstance == nullptr)
		sm_pInstance = new InputHandler();

	return *sm_pInstance;
}

void InputHandler::DestroyInstance()
{
	if (sm_pInstance != nullptr)
		delete sm_pInstance;

	sm_pInstance = nullptr;
}

void InputHandler::Update()
{
	sf::Event event;
	sf::RenderWindow* _window = Renderer::GetInstance().GetWindow();
	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			SceneManager::GetInstance().Quit();
		}
	}

	if (!IsKeyPressed(sf::Keyboard::Tab))
	{
		m_playerInControl = m_playerNext;
	}
}

sf::Keyboard::Key InputHandler::IsAnyKeyPressed()
{
	for (int i = 0; i < sf::Keyboard::Key::KeyCount; i++)
	{
		if (IsKeyPressed(sf::Keyboard::Key(i)))
		{
			return sf::Keyboard::Key(i);
		}
	}
	
	return sf::Keyboard::Key::Unknown;
}

sf::Vector2i InputHandler::GetMousePosition()
{
	return sf::Mouse::getPosition(*Renderer::GetInstance().GetWindow());
}

void InputHandler::SwitchCharacter(int current)
{
	if (current == m_playerInControl)
	{
		m_playerNext = current + 1;
		if (current == 4)
			m_playerNext = 1;
	}
}

//Constructor
InputHandler::InputHandler()
{

}

//Destructor
InputHandler::~InputHandler()
{
	
}