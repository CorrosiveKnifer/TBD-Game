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
#include <SFML/Audio.hpp>

// parent include
#include "Entity.h"

// local includes
#include "GlobalVariables.h"

class Entity;

class C_Ball : Entity
{
public:
	C_Ball(b2World* world, unsigned int playerID, sf::Vector2f _worldPosition, b2Vec2 _vectorVelocity,bool isTemporary = false);
	virtual ~C_Ball();

	//Inheritance functions:
	virtual void Draw();
	virtual void Process(float dT);
	virtual void HandleHit(Entity* other);

	const b2Body* GetBody() { return MyBox2d.BOD; };
	int GetPlayerID() { return this->myPlayerID; }
	int GetBounceCount() { return this->m_bounceCount; }
	virtual bool IsImmune() { return m_bounceCount == 0; };
private:
	unsigned int myPlayerID;

	// the ball graphics
	sf::Texture Tx_MyBall;
	sf::Sprite Spr_Ball;
	sf::Color myBallColor;
	sf::Color m_immuneColor;

	// sound
	sf::SoundBuffer SB_ball_Hit;
	sf::Sound S_ball_Hit;

	float mf_SpeedModifier = 50.0f;
	unsigned int m_bounceCount = 0;
	unsigned int m_bounceMax = 6;
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