#ifndef _ControlsScene_H_
#define _ControlsScene_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: ControlsScene.h
// Description   	: Scene for the controls menu.
// Author         	: Bailey Johnson
// Mail         	: bailey.johnson@mds.ac.nz
//
//Parent Include
#include "Scene.h"

//Library Includes
#include <vector>

//Forward Declaration
class Button;

//Implementation
class ControlsScene : public Scene
{
	//Member function
public:
	ControlsScene();
	virtual ~ControlsScene();

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

	Button* m_pBackBtn;

	bool m_hasMouseClicked;

	//Test variable
	int keypressed;
};
#endif //_ControlsScene_H_


