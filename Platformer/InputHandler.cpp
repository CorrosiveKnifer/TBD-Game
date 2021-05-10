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

//Constructor
InputHandler::InputHandler()
{

}

//Destructor
InputHandler::~InputHandler()
{
	
}

// Get Any Input from a Joystick
int InputHandler::GetAnyJoystickInput()
{

}

// Get the aiming direction input from the joystick
sf::Vector2i InputHandler::GetAimInput(int joystickID)
{
	return sf::Vector2i();
}

// Get the movement direction input from the joystick
sf::Vector2i InputHandler::GetMovementInput(int joystickID)
{
	return sf::Vector2i();
}

// Get the shoot input from the joystick
float InputHandler::GetShootInput(int joystickID)
{
	return 0.0f;
}

// Get the emote input from the joystick
int InputHandler::GetEmoteInput(int joystickID)
{
	return 0;
}

// Return the amount of joysticks connected
int InputHandler::GetJoystickCount()
{
	return 0;
}

std::initializer_list<InputHandler::ButtonType> InputHandler::GetButtonInput(int joystickID)
{
	return std::initializer_list<ButtonType>();
}


// INITIALISE AND INFORMATION

//// Is joystick #0 connected?
//
//for (int i = 0; i < 5; i++)
//{
//	bool connected = sf::Joystick::isConnected(i);
//	sf::Joystick::Identification controllerID = sf::Joystick::getIdentification(i);
//	if (connected)
//	{
//		std::cout << "Controller Detected" << std::endl;
//		std::cout << "\nVendor ID: " << controllerID.vendorId << "\nProduct ID: " << controllerID.productId << std::endl << std::endl;
//	}
//}
//
//
//// How many buttons does joystick #0 support?
//unsigned int buttons = sf::Joystick::getButtonCount(1);
//std::cout << buttons << std::endl;
//// Does joystick #0 define a X axis?
//bool hasX = sf::Joystick::hasAxis(0, sf::Joystick::X);
//// Is button #2 pressed on joystick #0?
//bool pressed = sf::Joystick::isButtonPressed(0, 2);
//// What's the current position of the Y axis on joystick #0?
//float position = sf::Joystick::getAxisPosition(0, sf::Joystick::Y);