#ifndef _SHIELD_
#define _SHIELD_

// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: Shield.h
// Description   	: Shield Class for the power up activated by a player.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//

#include "Entity.h"
class Shield :    public Entity
{
public:
	Shield(b2World* world, unsigned int playerID, b2Vec2 _worldPosition);
	virtual ~Shield();
	virtual void Draw();
	virtual void Process(float dT);
	virtual void HandleHit(Entity* other) {};

	void SetPosition(b2Vec2 pos) { MyBox2d.BOD->SetTransform(pos, 0); };
private:

protected:

	//Member Data
public:

private:

	sf::Sprite* m_bodySprite;

	// box2d
	struct body
	{
		b2BodyDef DEF;
		b2CircleShape SHAPE;
		b2FixtureDef FIX;
		b2Body* BOD;
	};

	body MyBox2d;
protected:

};

#endif