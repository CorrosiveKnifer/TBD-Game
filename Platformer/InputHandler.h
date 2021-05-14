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

//Forward Declaration
class BackBuffer;
class Game;
class LogoScene;
class Window;

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

	void SwitchCharacter(int current);
private:
	InputHandler();
	~InputHandler();
protected:

	//Member data
public:
	int m_playerInControl = 1;
	int m_playerNext = 1;
private:
	static InputHandler* sm_pInstance;

protected:

};
#endif // _INPUT_HANDLER_H_