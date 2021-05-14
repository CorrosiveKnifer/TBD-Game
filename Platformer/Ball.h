#pragma once
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: Player_Entity.h
// Description   	: Player Class for any player.
// Author         	: Sonja Fowler
// Mail         	: sonja@alp.co.nz
//

#ifndef _BALL_ENTITY_
#define _BALL_ENTITY_

// Library Includes
#include <Box2D.h>
#include <SFML/Graphics.hpp>

// parent include
#include "Entity.h"

// local includes
#include "GlobalVariables.h"

class C_Player;

class C_Ball : Entity
{
public:
	C_Ball(b2World* world, C_Player* _ownerOfThisBall, b2Vec2 _worldPosition, b2Vec2 _vectorVelocity);
	virtual ~C_Ball();

	//Inheritance functions:
	virtual void Draw();
	virtual void Process(float dT);

private:
	C_Player* OwnerOfThisBall;

	// the ball graphics
	sf::Texture Tx_MyBall;
	sf::Sprite Spr_Ball;
	sf::Color myBallColor;

	float mf_SpeedModifier = 100.0f;

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