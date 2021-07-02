#ifndef _PlayerSelectScene_H_
#define _PlayerSelectScene_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: PlayerSelectScene.h
// Description   	: Scene for players to join the game with their controller
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
class PlayerSelectScene : public Scene
{
	//Member function
public:
	PlayerSelectScene();
	virtual ~PlayerSelectScene();

	//Inherited functions:
	virtual void Draw();
	virtual void Update(float dT);
	virtual void DestroyEntity(Entity* entity) {};

	//Button functions:
	void Play();
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

	sf::Texture player1Tex;
	sf::Sprite player1Spr;

	sf::Texture player2Tex;
	sf::Sprite player2Spr;

	sf::Texture player3Tex;
	sf::Sprite player3Spr;

	sf::Texture player4Tex;
	sf::Sprite player4Spr;

	sf::Sprite* m_yButton;
	Button* m_pPlayBtn;
	Button* m_pBackBtn;

	bool m_hasMouseClicked;

	int menuSelection = 1;

	//Test variable
	int keypressed;	
};
#endif //_PlayerSelectScene_H_
