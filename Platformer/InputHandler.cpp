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
		if (event.type == sf::Event::JoystickButtonPressed)
		{
			int joyID = event.joystickButton.joystickId;
			int joyBtn = event.joystickButton.button;

			GetButtonInput(joyID, joyBtn);
		}
		if (event.type == sf::Event::JoystickMoved)
		{
			int joyID = event.joystickMove.joystickId;
			sf::Joystick::Axis joyAxis = event.joystickMove.axis;
			float joyPos = event.joystickMove.position;

			GetAnyJoystickInput(joyID, joyAxis, joyPos);
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
//sf::Vector2i InputHandler::GetAimInput(int joystickID, sf::Joystick::Axis axis, float pos)
//{
//	return sf::Vector2i();
//}
//
//// Get the movement direction input from the joystick
//sf::Vector2i InputHandler::GetMovementInput(int joystickID, sf::Joystick::Axis axis, float pos)
//{
//	return sf::Vector2i();
//}
//
//// Get the shoot input from the joystick
//float InputHandler::GetShootInput(int joystickID, int button)
//{
//	return 0.0f;
//}
//
//// Get the emote input from the joystick
//int InputHandler::GetEmoteInput(int joystickID, sf::Joystick::Axis axis, float pos)
//{
//	return 0;
//}

// Return the amount of joysticks connected
int InputHandler::GetJoystickCount()
{

	return 0;
}

// Get Any Input from a Joystick
void InputHandler::GetAnyJoystickInput(int joystickID, sf::Joystick::Axis axis, float pos)
{
	switch (axis)
	{
	case sf::Joystick::X:
		// MOVEMENT - LEFT JOYSTICK - LEFT (-X) AND RIGHT (X)

		break;
	case sf::Joystick::Y:
		// MOVEMENT - LEFT JOYSTICK - UP (-Y) AND DOWN (Y)

		break;
	case sf::Joystick::R:
		// AIM - RIGHT JOYSTICK - UP (-R) AND DOWN (R)

		break;
	case sf::Joystick::U:
		// AIM - RIGHT JOYSTICK - LEFT (-U) AND RIGHT (U)

		break;
	case sf::Joystick::PovX:
		// EMOTE - DPAD - LEFT (-PovX) AND RIGHT (PovX)

		break;
	case sf::Joystick::PovY:
		// EMOTE - DPAD - UP (PovY) AND DOWN (-PovY)
		break;
	case sf::Joystick::Z:
		// SHOOT - LEFT TRIGGER (Z) AND RIGHT TRIGGER (-Z)

		break;
	default:
		break;
	}
}

void InputHandler::GetButtonInput(int joystickID, int button)
{
	switch (button)
	{
	case BUTTON_A:
		// JUMP
		
		break;
	case BUTTON_B:
		// DODGE

		break;
	case BUTTON_X:
		// SHOW BALL

		break;
	case BUTTON_Y:
		// POWER UP

		break;
	case BUTTON_LB:
		// DODGE

		break;
	case BUTTON_RB:
		// JUMP
		break;
	case BUTTON_BACK:
		// NOTHING

		break;
	case BUTTON_START:
		// PAUSE

		break;
	case BUTTON_L3:
		// NOTHING

		break;
	case BUTTON_R3:
		// NOTHING

		break;
	default:
		break;
	}
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