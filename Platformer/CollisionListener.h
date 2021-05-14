#pragma once
#include <Box2D.h>
#include <typeinfo>
#include "Player_Entity.h"
#include "Ball.h"

class CollisionListener : public b2ContactListener
{
public:
	virtual void BeginContact(b2Contact* contact)
	{
		Entity* entityA = reinterpret_cast<Entity*>(contact->GetFixtureA()->GetUserData().pointer);
		Entity* entityB = reinterpret_cast<Entity*>(contact->GetFixtureB()->GetUserData().pointer);

		if (entityA == nullptr || entityB == nullptr)
			return;

		if (typeid(*entityA) == typeid(C_Player) && typeid(*entityB) == typeid(C_Player))
			return;
		if (typeid(*entityA) == typeid(C_Ball) && typeid(*entityB) == typeid(C_Ball))
			return;
		
		if (typeid(*entityA) == typeid(C_Player) && typeid(*entityB) == typeid(C_Ball))
		{
			if (!entityA->IsImmune())
			{
				entityA->HandleHit();
				entityB->HandleHit();
			}
			return;
		}
		if (typeid(*entityA) == typeid(C_Ball) && typeid(*entityB) == typeid(C_Player))
		{
			if (!entityB->IsImmune())
			{
				entityB->HandleHit();
				entityA->HandleHit();
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

		if (typeid(*entityA) == typeid(C_Player) && typeid(*entityB) == typeid(C_Player))
			return;
		if (typeid(*entityA) == typeid(C_Ball) && typeid(*entityB) == typeid(C_Ball))
			return;

		if (typeid(*entityA) == typeid(C_Player) && typeid(*entityB) == typeid(C_Ball))
		{
			if(entityA->IsImmune())
			{
				contact->SetEnabled(false);
			}
			return;
		}
		if (typeid(*entityA) == typeid(C_Ball) && typeid(*entityB) == typeid(C_Player))
		{
			if (entityB->IsImmune())
			{
				contact->SetEnabled(false);
			}
			return;
		}
	}

	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) 
	{

	}
};