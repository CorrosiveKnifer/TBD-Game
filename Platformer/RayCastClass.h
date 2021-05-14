#pragma once
#include <Box2D.h>
#include <vector>

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
		rayHits.push_back(RaycastHit(fixture, point, normal));

		return 1.0f;
	}
};