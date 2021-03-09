#ifndef _PARTICLE_H_
#define _PARTICLE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	ParticleADT.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	ParticleADT
// Description:
//		Particle class template type for use within the ParticleSystem.
//

//Parent Inludes
#include "Drawable.h"

//Local Includes
#include "Mesh.h"
#include "Texture.h"
#include "Program.h"

class ParticleADT : public Drawable
{	
	//Member Functions:
public:
	ParticleADT() {};
	virtual ~ParticleADT() {};

	virtual void Draw(Camera* camera, Program* program) = 0;
	virtual void Process(float dT) = 0;

	virtual bool IsDead()
	{
		//If a limit is set to zero or less, then it isn't a limitation.
		bool DistanceLimit = (m_displacement >= m_DistLimit && m_DistLimit > 0.0f);

		bool TimeLimit = (m_time >= m_lifeTime && m_lifeTime > 0.0f);

		if (m_DistLimit <= 0.0f)
		{
			return TimeLimit;
		}

		if (m_lifeTime <= 0.0f)
		{
			return DistanceLimit;
		}

		return  DistanceLimit && TimeLimit;
	};

	virtual void SetBaseScale(glm::vec3 scale) { m_baseScale = scale; };
protected:

private:

	//Member Data:
public:

protected:
	Mesh* m_pMesh = 0;							//Base mesh of this particle
	glm::vec3 m_direction = glm::vec3();		//Direction of this particle

	float m_width = 0;							//Width of this particle's mesh.
	float m_height = 0;							//Height of this particle's mesh.
	float m_rotationStep = 0.0f;				//Rotation of this particle per update step
	float m_speed = 0.0f;						//Speed of this particle
	
	float m_DistLimit = 0.0f;					//Maximum distance of this particle before death.
	float m_lifeTime = 0.0f;					//Time Limit of this particle before death.

	float m_time = 0.0f;						//Current life time of this particle.
	float m_displacement = 0.0f;				//Current distance traveled by this particle.

	glm::vec3 m_baseScale;						//Base scale to reference the other scale off.
private:

};

#endif