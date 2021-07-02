#ifndef _RAYCASTING_H_
#define _RAYCASTING_H_

// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: RayCastClass.h
// Description   	: Raycasting handler to use with Box2d
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//

//Library include
#include <Box2D.h>
#include <vector>

//Local Include
#include "Entity.h"

struct RaycastHit
{
	RaycastHit(b2Fixture* _fixture, const b2Vec2& _point, const b2Vec2& _normal)
	{
		fixture = _fixture;
		contactPoint = _point;
		contactNormal = _normal;
	}

	b2Fixture* fixture;
	b2Vec2 contactPoint;
	b2Vec2 contactNormal;
};

class RayCastClass : public b2RayCastCallback
{
public:
	std::vector<RaycastHit> rayHits;

	float ReportFixture(b2Fixture* fixture, const b2Vec2& point, const b2Vec2& normal, float fraction)
	{
		Entity* entityA = reinterpret_cast<Entity*>(fixture->GetUserData().pointer);

		if(entityA == nullptr)
			rayHits.push_back(RaycastHit(fixture, point, normal));

		return 1.0f;
	}
};

#endif