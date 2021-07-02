#ifndef _MainMenuScene_H_
#define _MainMenuScene_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: MainMenuScene.h
// Description   	: A class for the Main menu, processed before the game is played.
// Author         	: Michael Jordan, Bailey Johnson
// Mail         	: michael.jor8834@mediadesign.com, bailey.johnson@mds.ac.nz
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
	virtual void Draw();
	virtual void Update(float dT);
	virtual void DestroyEntity(Entity* entity) {};

	//Button functions:
	void PlayerSelect();
	void Controls();
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

	sf::Texture menuBackgroundTex;
	sf::Sprite menuBackgroundSpr;

	Button* m_pPlayerSelectBtn;
	Button* m_pControlsBtn;
	Button* m_pSettingsBtn;
	Button* m_pQuitBtn;

	bool m_hasMouseClicked;

	int menuSelection = 1;

	//Test variable
	int keypressed;
};
#endif //_MainMenuScene_H_