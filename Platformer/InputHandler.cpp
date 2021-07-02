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
#include "Player_Entity.h"
#include "PlayerSelectScene.h"

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

	if (SceneManager::GetInstance().m_topScene == nullptr)
		return;

	while (_window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			SceneManager::GetInstance().Quit();
		}
		// Bailey - Controller button inputs
		if (event.type == sf::Event::JoystickButtonPressed && sf::Joystick::getIdentification(event.joystickButton.joystickId).vendorId != 0x054C)
		{
			int joyID = event.joystickButton.joystickId;
			int joyBtn = event.joystickButton.button;

			if (joyBtn == BUTTON_Y && SceneManager::GetInstance().m_topScene->sceneID == PLAYERSELECT || joyBtn == BUTTON_B && SceneManager::GetInstance().m_topScene->sceneID == PLAYERSELECT)
			{
				if (joyBtn == BUTTON_Y && playerJoystickIDs.size() < 4)
				{
					if (std::find(playerJoystickIDs.begin(), playerJoystickIDs.end(), joyID) == playerJoystickIDs.end())
					{
							playerJoystickIDs.push_back(joyID);		
					}
				}
				if (joyBtn == BUTTON_B && playerJoystickIDs.size() >= 1 && std::find(playerJoystickIDs.begin(), playerJoystickIDs.end(), joyID) != playerJoystickIDs.end())
				{
					playerJoystickIDs.erase(std::find(playerJoystickIDs.begin(), playerJoystickIDs.end(), joyID));
				}
			}

			if (SceneManager::GetInstance().m_topScene->sceneID == CONTROLS || 
				SceneManager::GetInstance().m_topScene->sceneID == SETTINGS || 
				SceneManager::GetInstance().m_topScene->sceneID == MAINMENU || 
				SceneManager::GetInstance().m_topScene->sceneID == PLAYERSELECT || 
				SceneManager::GetInstance().m_topScene->sceneID == VICTORY)
			{
				if (joyBtn == BUTTON_A)
				{
					SceneManager::GetInstance().m_topScene->buttonPressed = true;
				}
			}
		}
		// Bailey - Controller Joystick Inputs
		if (event.type == sf::Event::JoystickMoved && sf::Joystick::getIdentification(event.joystickButton.joystickId).vendorId != 0x054C)
		{
			int joyID = event.joystickMove.joystickId;
			sf::Joystick::Axis joyAxis = event.joystickMove.axis;
			float joyPos = event.joystickMove.position;	

			if (SceneManager::GetInstance().m_topScene->sceneID == CONTROLS ||
				SceneManager::GetInstance().m_topScene->sceneID == SETTINGS ||
				SceneManager::GetInstance().m_topScene->sceneID == MAINMENU ||
				SceneManager::GetInstance().m_topScene->sceneID == PLAYERSELECT)
			{
				// Right
				if ((joyAxis == sf::Joystick::Axis::PovX) && joyPos > 50.0f)
				{					
					SceneManager::GetInstance().m_topScene->rightPressed = true;
				}

				// Up
				if ((joyAxis == sf::Joystick::Axis::PovY) && joyPos > 50.0f)
				{
					SceneManager::GetInstance().m_topScene->upPressed = true;
				}

				// Left
				if ((joyAxis == sf::Joystick::Axis::PovX) && joyPos < -50.0f)
				{
					SceneManager::GetInstance().m_topScene->leftPressed = true;
				}
				// Down
				if ((joyAxis == sf::Joystick::Axis::PovY) && joyPos < -50.0f)
				{
					SceneManager::GetInstance().m_topScene->downPressed = true;
				}
			}
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

//// Get the aiming direction input from the joystick
sf::Vector2f InputHandler::GetAimInput(int joystickID)
{
	return sf::Vector2f(sf::Joystick::getAxisPosition(joystickID, sf::Joystick::Axis::U), sf::Joystick::getAxisPosition(joystickID, sf::Joystick::Axis::V));
}

// Get the movement direction input from the joystick
sf::Vector2f InputHandler::GetMovementInput(int joystickID)
{	
	return sf::Vector2f(sf::Joystick::getAxisPosition(joystickID, sf::Joystick::Axis::X), sf::Joystick::getAxisPosition(joystickID, sf::Joystick::Axis::Y));
}

// Get the shoot input from the joystick
float InputHandler::GetShootInput(int joystickID)
{
	return sf::Joystick::getAxisPosition(joystickID, sf::Joystick::Axis::Z);
}

// Get the emote input from the joystick
sf::Vector2f InputHandler::GetEmoteInput(int joystickID)
{
	return sf::Vector2f(sf::Joystick::getAxisPosition(joystickID, sf::Joystick::Axis::PovX), sf::Joystick::getAxisPosition(joystickID, sf::Joystick::Axis::PovY));
}

// Return the amount of joysticks connected
int InputHandler::GetJoystickCount()
{
	for (int i = 0; i < 8; i++)
	{
		bool connected = sf::Joystick::isConnected(i);
		sf::Joystick::Identification controllerID = sf::Joystick::getIdentification(i);
		if (connected)
		{
			std::cout << "Controller Detected" << std::endl;
			std::cout << "\nVendor ID: " << controllerID.vendorId << "\nProduct ID: " << controllerID.productId << std::endl << std::endl;
		}
	}
	return sf::Joystick::Count;
}

std::list<InputHandler::ButtonType> InputHandler::getButton(int joystickID)
{

	return std::list<ButtonType>();
}