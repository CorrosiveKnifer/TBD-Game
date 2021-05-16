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

class Entity;

class C_Ball : Entity
{
public:
	C_Ball(b2World* world, unsigned int playerID, sf::Vector2f _worldPosition, b2Vec2 _vectorVelocity);
	virtual ~C_Ball();

	//Inheritance functions:
	virtual void Draw();
	virtual void Process(float dT);
	virtual void HandleHit(Entity* other);

	const b2Body* GetBody() { return MyBox2d.BOD; };
private:
	unsigned int myPlayerID;

	// the ball graphics
	sf::Texture Tx_MyBall;
	sf::Sprite Spr_Ball;
	sf::Color myBallColor;

	float mf_SpeedModifier = 50.0f;

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