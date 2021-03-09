#ifndef _PARTICLE_SYSTEM_H_
#define _PARTICLE_SYSTEM_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	ParticleSystem.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	ParticleSystem
// Description:
//		Templated System that creates particles onto the scene.
//

//Local Includes
#include "Renderer.h" //<--- Dependencies Includes
#include "ParticleADT.h"

//Library Includes
#include <vector>
#include <utility>

//Forward Declaration
class Mesh;

//Restrict T to only particle subclasses
template<typename T, typename std::enable_if<std::is_base_of<ParticleADT, T>::value>::type* = nullptr>
class ParticleSystem
{
	//Member Functions:
public:

	//Default Constructor
	ParticleSystem()
	{
		m_isRunning = false;
		m_program = Renderer::GetInstance().CreateProgram("Particle.vs", "Particle.fs");
	};

	//Destructor
	~ParticleSystem()
	{
		std::vector<ParticleADT*>::iterator iter = m_pParticles.begin();
		while (iter != m_pParticles.end())
		{
			delete* iter;
			iter = m_pParticles.erase(iter);
		}
	};

	//	Draw( )
	//
	//	Author: Michael Jordan
	//	Description:
	//		Draws all alive particles.
	//		
	//	Param:
	//		- n/a	|	
	//
	//	Return: n/a		|
	//
	void Draw(Camera* camera)
	{
		std::vector<ParticleADT*>::iterator iter = m_pParticles.begin();
		while (iter != m_pParticles.end())
		{
			if (!(*iter)->IsDead())
			{
				(*iter)->Draw(camera, m_program);
			}
			
			iter++;
		}
	};

	//	Process( inheritedMatrix, dT )
	//
	//	Author: Michael Jordan
	//	Description:
	//		Process all particles and decides if a new one should be created.
	//		
	//	Param:
	//		- glm::mat4	|	Inherited Matrix to create matrix upon.
	//		- float		|	delta time since last process call.
	//
	//	Return: n/a		|
	//
	void Process(float dT)
	{
		if (m_isRunning)
		{
			std::vector<ParticleADT*>::iterator iter = m_pParticles.begin();
			while (iter != m_pParticles.end())
			{
				(*iter)->Process(dT);

				if ((*iter)->IsDead() && m_canRecycle)
				{
					iter = m_pParticles.erase(iter);
				}
				else
				{
					iter++;
				}
			}

			m_isRunning = !(m_pParticles.size() >= m_particleLimit) && !m_canRecycle;

			m_time += dT;
			while (m_time >= m_spawnTimer && m_pParticles.size() < m_particleLimit)
			{
				m_time -= m_spawnTimer;
				T* particle = new T();
				dynamic_cast<ParticleADT*>(particle)->SetLocalPosition(m_position);
				dynamic_cast<ParticleADT*>(particle)->SetLocalRotation(m_rotation);
				dynamic_cast<ParticleADT*>(particle)->SetLocalScale(m_scale);
				dynamic_cast<ParticleADT*>(particle)->SetBaseScale(m_scale);
				m_pParticles.push_back(particle);
			}
		}
	};

	void Start(){m_isRunning = true;};
	void Stop()	{ m_isRunning = false; };
	void Restart(){	m_time = 0.0f; Clear();	};

	bool IsFinished() { return !m_isRunning; };

	//Clear all particles currently in the system.
	void Clear()
	{
		std::vector<ParticleADT*>::iterator iter = m_pParticles.begin();
		while (iter != m_pParticles.end())
		{
			delete* iter;
			iter = m_pParticles.erase(iter);
		}
	};

	void SetLimit(unsigned int _limit) { m_particleLimit = (_limit > 0) ? _limit : m_particleLimit; };
	void SetSpawnTimer(float _timer) { m_spawnTimer = _timer; };
	void SetRecycling(bool _recycle) { m_canRecycle = _recycle;};

	void SetLocalPosition(glm::vec3 position) { m_position = position; };
	void SetLocalRotation(glm::vec3 rotation) { m_rotation = rotation; };
	void SetLocalScale(glm::vec3 scale) { m_scale = scale; };

	glm::vec3 GetLocalPosition() { return m_position; };
	glm::vec3 GetLocalRotation() { return m_rotation; };
	glm::vec3 GetLocalScale() { return m_scale; };
protected:

private:

	//Member Data:
public:

protected:
	std::vector<ParticleADT*> m_pParticles;
	unsigned int m_particleLimit = 100;

	float m_spawnTimer = 0.0f;
	float m_time = 0.0f;

	bool m_canRecycle = false;
	bool m_isRunning = false;

private:
	GLuint m_program;

	//Local
	glm::vec3 m_position = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
};

#endif


