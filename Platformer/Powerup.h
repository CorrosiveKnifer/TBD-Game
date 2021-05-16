#pragma once
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: Powerup.h
// Description   	: Powerup class for all powerups
// Author         	: Sonja Fowler
// Mail         	: sonja@alp.co.nz
//

#ifndef _POWERUP_ENTITY_
#define _POWERUP_ENTITY_

// Library Includes
#include <Box2D.h>
#include <SFML/Graphics.hpp>

// parent include
#include "Entity.h"

// local includes
#include "GlobalVariables.h"

class C_Player;

class C_PowerUp : Entity
{
public:
	C_PowerUp(b2World* world,b2Vec2 _worldPosition, unsigned int _PU_typeID);
	virtual ~C_PowerUp();

	//Inheritance functions:
	virtual void Draw();
	virtual void Process(float dT);

	const b2Body* GetBody() { return MyBox2d.BOD; };

private:

	// the ball graphics
	sf::Texture Tx_PowerupBottom, Tx_PowerupTop;
	sf::Sprite Spr_PowerUp_Bottom, Spr_PowerUp_Top;

	float mf_Powerup_FlashingTimer = 0.f;
	float mf_Powerup_Flashing = 2.0f;
	int mi_Powerup_FlashDirection = -1;
	sf::Color myColor;

	// box2d
	struct body
	{
		b2BodyDef DEF;
		b2CircleShape SHAPE;
		b2FixtureDef FIX;
		b2Body* BOD;
	};

	body MyBox2d;
};

#endif