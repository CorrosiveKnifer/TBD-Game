#ifndef _INPUT_HANDLER_H_
#define _INPUT_HANDLER_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: InputHandler.h
// Description   	: Handles the input from the keyboard and mouse on the window.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Library Includes
#include <SFML/Graphics.hpp>
#include <list>
#include "Player_Entity.h"

//Forward Declaration
class BackBuffer;
class LogoScene;

//Implementation
class InputHandler
{
	//Member function
public:
	static InputHandler& GetInstance();
	static void DestroyInstance();

	void Update();

	bool IsKeyPressed(sf::Keyboard::Key _key) { return sf::Keyboard::isKeyPressed(_key); };
	bool IsMousePressed(sf::Mouse::Button _button) { return sf::Mouse::isButtonPressed(_button);};
	sf::Keyboard::Key IsAnyKeyPressed();
	sf::Vector2i GetMousePosition();

//	enum ButtonType {
//		SHOOT,
//		JUMP,
//		SHOW_BALL,
//		POWER_UP,
//		DODGE,
//		PAUSE
//	};

	enum ButtonType {
		BUTTON_A = 0,
		BUTTON_B = 1,
		BUTTON_X = 2,
		BUTTON_Y = 3,
		BUTTON_LB = 4,
		BUTTON_RB = 5,
		BUTTON_BACK = 6,
		BUTTON_START = 7,
		BUTTON_L3 = 8,
		BUTTON_R3 = 9
	};

	int GetJoystickCount();

	//sf::Vector2i GetAimInput(int joystickID, sf::Joystick::Axis axis, float pos);
	//sf::Vector2i GetMovementInput(int joystickID, sf::Joystick::Axis axis, float pos);
	//float GetShootInput(int joystickID, int button);
	//int GetEmoteInput(int joystickID, sf::Joystick::Axis axis, float pos);
	

	void GetAnyJoystickInput(int joystickID, sf::Joystick::Axis axis, float pos);
	void GetButtonInput(int joystickID, int button);

	void SwitchCharacter(int current);

	C_Player* player;

	int m_playerInControl;
	int m_playerNext;

private:
	InputHandler();
	~InputHandler();
protected:

	//Member data
public:

private:
	static InputHandler* sm_pInstance;

protected:

};
#endif // _INPUT_HANDLER_H_