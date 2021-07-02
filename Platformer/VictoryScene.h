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
class VictoryScene : public Scene
{
	//Member function
public:
	VictoryScene();
	virtual ~VictoryScene();

	//Inherited functions:
	virtual void Draw();
	virtual void Update(float dT);
	virtual void DestroyEntity(Entity* entity) {};

	//Button functions:
	void Back();
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

	sf::Texture victorPlayerTex;
	sf::Sprite victorPlayerSpr;

	Button* m_pBackBtn;

	bool m_hasMouseClicked;

	float m_highscore;
	//Test variable
	int keypressed;
};
#endif //_VictoryScene_H_