#ifndef _MainMenuScene_H_
#define _MainMenuScene_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: MainMenuScene.h
// Description   	: A class for the Main menu, processed before the game is played.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Include
#include "Scene.h"

//Library Includes
#include <vector>

//Forward Declaration
class Button;

//Implementation
class MainMenuScene : public Scene
{
	//Member function
public:
	MainMenuScene();
	virtual ~MainMenuScene();
	
	//Inherited functions:
	//virtual bool DoWindowLoop();
	virtual void Draw();
	virtual void Update(float dT);

	//Button functions:
	void Play();
	void Select();
	void Settings();
	void Quit();
private:
	
protected:
	//Member data
public:
	
private:

protected:
	sf::Vector2f m_mousePos;
	sf::Vector2f* m_mousePressPos;

	Button* m_pPlayBtn;
	Button* m_pSelectBtn;
	Button* m_pSettingBtn;
	Button* m_pQuitBtn;

	bool m_hasMouseClicked;

	//Test variable
	int keypressed;
};
#endif //_MainMenuScene_H_