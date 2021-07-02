#ifndef _ENTITY_H_
#define _ENTITY_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: Entity.h
// Description   	: Parent class for playable, non-playable or objects within 
//					  the game world.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Library Includes
#include <cstdlib>

//Local Includes
#include "Renderer.h"
#include "InputHandler.h"

//Implementation
class Entity
{
	//Member Functions
public:

	Entity();
	virtual ~Entity();

	bool Initialise(sf::Drawable* _drawable);
	bool Deinitialise();

	//Inheritance functions:
	virtual void Draw() = 0;
	virtual void Process(float dT) = 0;
	virtual void HandleHit(Entity* other) = 0;
	virtual bool IsImmune() { return false; };

	const sf::Vector2f GetWorldPosition() const;
	const sf::Vector2f GetVelocity() const;
	
	void SetWorldPosition(float _x, float _y);
	void SetWorldPosition(sf::Vector2f _pos);
	void SetVelocity(sf::Vector2f _vel);

private:

protected:

	//Member Data
public:

private:

protected:
	sf::Drawable* m_pDrawable;
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
};
#endif // _ENTITY_H_