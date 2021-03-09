//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Explosion.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Explosion
// Description:
//		Custom particle for when the player or enemy are destroyed.
//

//Self Include
#include "Explosion.h"

//Local Include
#include "AudioManager.h"

//Constructor
Explosion::Explosion()
{
	//m_pMesh = new Mesh();
	//Renderer::GetInstance().CreateSphereMesh(*m_pMesh, 8, 8);
	//m_pMesh->SetTexture(Renderer::GetInstance().CreateTexture("DefaultTexture.png"));

	m_width = 1.0f;
	m_height = 1.0f;

	//Set variables
	m_lifeTime = 0.0f;
	m_DistLimit = 1.0f;
	m_speed = 3.0f;
	m_rotationStep = 0.0f;

	m_direction = glm::normalize(glm::vec3(rand() % 101 - 50, rand() % 101 - 50, rand() % 101 - 50));
	m_color = glm::vec3(1.0f, 1.0f, 1.0f);
	m_hasReached = false;
	m_hasPlayed = false;

	//Preprocess this particle.
	AudioManager::GetInstance().CreateSound("Explosion.wav", FMOD_DEFAULT);

	Process(0.0f);
}

//Destructor
Explosion::~Explosion()
{
	delete m_pMesh;
	m_pMesh = 0;
}

//	Draw( program )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Draws the particle in world space.
//		
//	Param:
//		- GLuint	|	Render program ID
//
//	Return: n/a		|
//
void Explosion::Draw(Camera* camera, Program* program)
{
	////Load shader:
	//program->Use();
	//
	////Load Matrix Information into the program.
	//program->SetMatrix4("CameraMat", camera->GetCamMatrix());
	//program->SetMatrix4("Transform", m_transform);
	//
	////Update alpha value in the program
	//program->SetVector4("Color", glm::vec4(m_color, 1.0f));
	//
	////Apply Texture
	//m_pMesh->GetTexture()->ApplyTexture(GL_TEXTURE0, GL_TEXTURE_2D, program);
	//
	////Draw Image
	//program->BindVertextArray(m_pMesh->GetVAO());
	//glDrawElements(GL_TRIANGLES, 6 * 8 * 8, GL_UNSIGNED_INT, 0);
	//
	////Clean up
	//program->Clear();
}

//	Process( inheritedMatrix, dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Process the variable status of this particle since the last call.
//		
//	Param:
//		- glm::mat4	|	Inherited Matrix to create matrix upon.
//		- float		|	delta time since last process call.
//
//	Return: n/a		|
//
void Explosion::Process(float dT)
{
	if (!IsDead())
	{
		//Move particle
		float distance = m_speed * dT;
		SetLocalPosition(m_position + glm::vec3(m_direction.x * distance, m_direction.y * distance, m_direction.z * distance));

		//Rotate particle
		SetLocalRotation(glm::vec3(0.0f, 0.0f, m_rotation.z + m_rotationStep * dT));

		float scale = m_displacement / m_DistLimit * 2.0f;
		scale += (m_hasReached) ? 2.0f: 0;
		SetLocalScale(m_baseScale * glm::vec3(scale, scale, scale));

		if (!m_hasPlayed)
		{
			AudioManager::GetInstance().PlaySoundEffect("Explosion.wav");
			m_hasPlayed = true;
		}

		//Update limits
		m_displacement += distance;
		m_time += dT;

		UpdateModelMatrix();
		m_pMesh->SetInheritedMatrix(m_transform);

		if (!m_hasReached)
		{
			//WHITE -> YELLOW
			m_color = glm::vec3(1.0f, 1.0f, 1.0f - (m_displacement / m_DistLimit));
		}
		else
		{
			//YELLOW -> RED
			m_color = glm::vec3(1.0f, 1.0f - (m_displacement / m_DistLimit), 0.0f);
		}
		
		if (!m_hasReached && m_displacement >= m_DistLimit)
		{
			m_displacement = 0.0f;
			m_hasReached = true;
		}
	}
}
