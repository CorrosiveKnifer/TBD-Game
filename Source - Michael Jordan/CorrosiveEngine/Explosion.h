#ifndef _EXPLOSION_PARTICLE_H_
#define _EXPLOSION_PARTICLE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Explosion.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Explosion
// Description:
//		Custom particle for when the player or enemy are destroyed.
//

//Parent Include
#include "ParticleADT.h"

class Explosion : public ParticleADT
{
	//Member Functions:
public:
	Explosion();
	virtual ~Explosion();

	virtual void Draw(Camera* camera, Program* program);
	virtual void Process(float dT);
protected:

private:

	//Member Data:
public:

protected:
	glm::vec3 m_color;

	bool m_hasReached;
	bool m_hasPlayed;
private:

};
#endif // !_EXPLOSION_PARTICLE_H_