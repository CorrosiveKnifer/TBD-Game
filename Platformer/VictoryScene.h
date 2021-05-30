#ifndef _VictoryScene_H_
#define _VictoryScene_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: VictoryScene.h
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
class VictoryScene : public Scene
{
	//Member function
public:
	VictoryScene();
	virtual ~VictoryScene();

	//Inherited functions:
	//virtual bool DoWindowLoop();
	virtual void Draw();
	virtual void Update(float dT);
	virtual void DestroyEntity(Entity* entity) {};

	//Button functions:
	void PlayerSelect();
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

	Button* m_pBackBtn;

	bool m_hasMouseClicked;

	//Test variable
	int keypressed;
};
#endif //_VictoryScene_H_