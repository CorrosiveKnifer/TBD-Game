//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	EntityCamera.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	EntityCamera
// Description:
//		Camera extended class which is specifically follows a single entity subclass class.
//

//Self Include
#include "EntityCamera.h"

//Constructor
EntityCamera::EntityCamera(CamType type)
	: m_isFollowing(false), m_pTarget(0)
{
	m_type = type;
	m_worldScale = glm::vec3(1.0f, 1.0f, 1.0f);

	SetUpDefault();
}

//Destructor
EntityCamera::~EntityCamera()
{
	//Not incharge of target
	m_pTarget = 0;
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes updates this camera's position to keep watching the entity,
//		and if enabled; will follow the entity.
//		
//	Param:
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void EntityCamera::Process(float dt)
{
	if (!m_isInTransition && m_pTarget != 0)
	{
		//Calculate change in movement:
		glm::vec3 newTargetPosition = m_pTarget->GetTransform().GetPosition();
		glm::vec3 movementVec = newTargetPosition - m_targetPos;
	
		//Update target's location
		m_targetPos = newTargetPosition;
		m_view.at = m_targetPos;

		//Move camera to keep track
		if (m_isFollowing)
			m_view.eye += movementVec;
	
		//Recalculate the view matrix
		CalculateViewMatrix();
	}
	
	Camera::Process(dt);
}
