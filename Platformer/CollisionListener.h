#pragma once
// system includes
#include <typeinfo>

// Library Includes
#include <Box2D.h>


// local includes
#include "Player_Entity.h"
#include "Ball.h"
#include "Powerup.h"
#include "GlobalVariables.h"
#include "SoundBuffer.h"

class CollisionListener : public b2ContactListener
{
public:
	enum EntityType
	{
		NONE	= 0x0000,
		PLAYER	= 0x0001,
		BALL	= 0x0002,
		POWERUP = 0x0004,
		SHIELD = 0x0008,
	};

	virtual void BeginContact(b2Contact* contact)
	{
		Entity* entityA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetUserData().pointer);
		Entity* entityB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetUserData().pointer);
		
		uint16 caseNo = GetType(entityA) | GetType(entityB);

		if (caseNo == (EntityType::PLAYER | EntityType::BALL))
		{

			entityA->HandleHit(entityB);
			entityB->HandleHit(entityA);
			return;
		}
		if (caseNo == (EntityType::BALL | EntityType::SHIELD))
		{
			entityA->HandleHit(entityB);
			entityB->HandleHit(entityA);
			return;
		}
		if (caseNo & EntityType::BALL)
		{
			if (GetType(entityA) == EntityType::BALL)
			{
				entityA->HandleHit(nullptr);
			}
			if (GetType(entityB) == EntityType::BALL)
			{
				entityB->HandleHit(nullptr);
			}
			return;
		}
	}

	virtual void EndContact(b2Contact* contact)
	{
		
	}

	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold)
	{
		Entity* entityA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetUserData().pointer);
		Entity* entityB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetUserData().pointer);

		if (entityA == nullptr || entityB == nullptr)
			return;
		if (typeid(*entityA) == typeid(*entityB))
			return;

		uint16 caseNo = GetType(entityA) | GetType(entityB);

		if (caseNo == (EntityType::PLAYER | EntityType::BALL))
		{
			if (entityA->IsImmune() || entityB->IsImmune())
			{
				contact->SetEnabled(false);
			}
			return;
		}
		if (caseNo == (EntityType::PLAYER | EntityType::POWERUP))
		{
			if (GetType(entityA) == EntityType::PLAYER)
			{
				entityB->HandleHit(entityA);
			}
			else
			{
				entityA->HandleHit(entityB);
			}
			contact->SetEnabled(false);
			return;
		}

		if (caseNo == (EntityType::BALL | EntityType::POWERUP))
		{
			if (entityA->IsImmune() || entityB->IsImmune())
			{
				contact->SetEnabled(false);
			}
		}
	}

	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) 
	{

	}

	uint16 GetType(Entity* entity)
	{
		if (entity == nullptr)
			return EntityType::NONE;

		if (typeid(*entity) == typeid(C_Player))
		{
			return EntityType::PLAYER;
		}
		if (typeid(*entity) == typeid(C_Ball))
		{
			return EntityType::BALL;
		}
		if (typeid(*entity) == typeid(C_PowerUp))
		{
			return EntityType::POWERUP;
		}
		if (typeid(*entity) == typeid(Shield))
		{
			return EntityType::POWERUP;
		}
		return EntityType::NONE;
	}
};