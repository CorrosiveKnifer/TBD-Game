#pragma once
#include <Box2D.h>
#include <typeinfo>
#include "Player_Entity.h"
#include "Ball.h"
#include "Powerup.h"
#include "GlobalVariables.h"

class CollisionListener : public b2ContactListener
{
public:
	enum EntityType
	{
		NONE	= 0x0000,
		PLAYER	= 0x0001,
		BALL	= 0x0002,
		POWERUP = 0x0004,
	};

	virtual void BeginContact(b2Contact* contact)
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
			entityA->HandleHit(entityB);
			entityB->HandleHit(entityA);

			// send score to owner of this ball
			C_Ball* entityTempBall = reinterpret_cast<C_Ball*>(entityB);
			switch (entityTempBall->GetPlayerID())
			{
				case  1:
					C_GlobalVariables::Player_1_Score += 10;
				break;
				case  2:
					C_GlobalVariables::Player_2_Score += 10;
				break;
				case  3:
					C_GlobalVariables::Player_3_Score += 10;
				break;
				case  4:
					C_GlobalVariables::Player_4_Score += 10;
				break;
			}
			// take life from this player that got hit
			C_Player* entityTempPlayer = reinterpret_cast<C_Player*>(entityA);
			entityTempPlayer->TakeLife();

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

		if (caseNo & (EntityType::PLAYER | EntityType::BALL))
		{
			if (entityA->IsImmune() || entityB->IsImmune())
			{
				contact->SetEnabled(false);
			}
			return;
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

		return EntityType::NONE;
	}
};