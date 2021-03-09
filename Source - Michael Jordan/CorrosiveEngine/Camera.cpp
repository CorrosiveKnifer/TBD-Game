//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Camera.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Camera
// Description:
//		Camera is preset to a type based on being perspective or orthographic.
//		Used to store the camera's view and projection matrix.
//

//Self Include
#include "Camera.h"

//Main constructor
Camera::Camera(CamType type)
	: m_currentProj(), m_type(type)
{
	m_worldScale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_shakeVector = glm::vec3(0.0f, 0.0f, 0.0f);
	m_isInTransition = false;
	SetUpDefault();
}

//Destructor
Camera::~Camera()
{
	//Text generators are deleted elsewhere.
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Applys any camera shake, since last call. 
//		
//	Param:
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void Camera::Process(float dt)
{
	if (m_isInTransition)
	{
		glm::vec3 eyeInterp = glm::mix(m_view.eye, m_targetView.eye, dt);
		glm::vec3 atInterp = glm::mix(m_view.at, m_targetView.at, dt);
		glm::vec3 upInterp = glm::mix(m_view.up, m_targetView.up, dt);

		SetView(View(eyeInterp, atInterp, upInterp));

		if (m_view.IsEqual(m_targetView, 0.1f))
		{
			m_isInTransition = false;
			m_view = m_targetView;
		}
	}

	//glm::vec3 shake;
	//shake.x = (rand() % 200 - 100) / 100.0f * m_shakeVector.x; //-100% to 100%
	//shake.y = (rand() % 200 - 100) / 100.0f * m_shakeVector.y; //-100% to 100%
	//shake.z = (rand() % 200 - 100) / 100.0f * m_shakeVector.z; //-100% to 100%
	//
	//m_shakeVector.x -= abs(shake.x);
	//m_shakeVector.y -= abs(shake.y);
	//m_shakeVector.z -= abs(shake.z);
	//
	//m_viewMat = glm::lookAt(m_pos + shake, m_targetPos + shake, m_up);
}

//	SetProjection( projection)
//
//	Author: Michael Jordan
//	Description:
//		Updates the all projection variables if and only if the new POV is more than zero.
//		
//	Param:
//		- Proj		|	Projecion structure
//
//	Return: n/a		|	
//
void Camera::SetProjection(Proj projection)
{
	//Clamp above zero
	m_currentProj = (projection.POV > 0) ? projection : m_currentProj;

	//Update Projection matrix
	CalculateProjection();
}

//	SetPOV( pov )
//
//	Author: Michael Jordan
//	Description:
//		Updates the point of view if and only if the new POV is more than zero.
//		
//	Param:
//		- float		|	Point of view
//
//	Return: n/a		|	
//
void Camera::SetPOV(float pov)
{
	//Clamp above zero
	m_currentProj.POV = (pov > 0) ? pov : m_currentProj.POV;

	//Update Projection matrix
	CalculateProjection();
}

//	GetCamMatrix( )
//
//	Author: Michael Jordan
//	Description:
//		Returns the projection times the view matrix.
//		
//	Param:
//		- n/a	|
//
//	Return: glm::mat4	|	Camera Matrix
//
const glm::mat4 Camera::GetCamMatrix()
{
	return m_projMat * glm::scale(glm::mat4(), m_worldScale) * m_viewMat;
}

//	GetWorldPosition( )
//
//	Author: Michael Jordan
//	Description:
//		Get the current position in world space of the camera.
//		
//	Param:
//		- n/a	|
//
//	Return: glm::vec3	|	Camera position
//
const glm::vec3 Camera::GetWorldPosition()
{
	return m_view.eye;
}

//	RemakeMatrix( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Remakes the projection matrix based on the new screen width and screen height
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void Camera::RemakeMatrix()
{
	//New values
	m_currentProj.width = Renderer::SCREEN_WIDTH;
	m_currentProj.height = Renderer::SCREEN_HEIGHT;

	CalculateProjection();
}

//	Shake( mag, activeAxis)
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Adds shake to be processed within the process function
//		
//	Param:
//		- float		|	magintude of the shake vector
//		- glm::vec3	|	vector to shake by
//
//	Return: n/a		|
//
void Camera::Shake(float mag, glm::vec3 activeAxis)
{
	activeAxis.x = abs(activeAxis.x);
	activeAxis.y = abs(activeAxis.y);
	activeAxis.z = abs(activeAxis.z);

	m_shakeVector = mag * activeAxis;
}

void Camera::TransitionView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up)
{
	m_targetView = View(_eye, _at, _up);
	m_isInTransition = true;
}

//	SetUpDefault( )
//
//	Author: Michael Jordan
//	Description:
//		Sets the default variables of the projection and view matrix
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void Camera::SetUpDefault()
{
	//Default:
	SetView(
		glm::vec3(0.0f, 0.0f, 1.0f),	//m_eye
		glm::vec3(0.0f, 0.0f, 0.0f),	//m_at
		glm::vec3(0.0f, 1.0f, 0.0f)		//m_up
	);

	//Default Projection
	m_currentProj.width = Renderer::SCREEN_WIDTH;
	m_currentProj.height = Renderer::SCREEN_HEIGHT;
	m_currentProj.nearDist = 0.1f;
	m_currentProj.farDist = 10000.0f;
	m_currentProj.POV = 45.0f;

	CalculateViewMatrix();
	CalculateProjection();
}

//	CalculateViewMatrix( )
//
//	Author: Michael Jordan
//	Description:
//		Updates the current view matrix.
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void Camera::CalculateViewMatrix()
{
	m_viewMat = glm::lookAt(m_view.eye, m_view.at, m_view.up);
}

//	CalculateProjection( )
//
//	Author: Michael Jordan
//	Description:
//		Updates the current projection matrix.
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void Camera::CalculateProjection()
{
	float aspectRatio;
	float halfWidth, halfHeight;

	switch (m_type)
	{
	default:
	case CamType::Perspective:
		aspectRatio = (float)m_currentProj.width / (float)m_currentProj.height;
		m_projMat = glm::perspective(m_currentProj.POV, aspectRatio, m_currentProj.nearDist, m_currentProj.farDist);
		break;

	case CamType::Orthographic:
		halfWidth = m_currentProj.width / 2.0f ;
		halfHeight = m_currentProj.height / 2.0f;
		m_projMat = glm::ortho(-halfWidth, halfWidth, -halfHeight, halfHeight, m_currentProj.nearDist, m_currentProj.farDist);
		break;
	}
}

//	SetPosition( )
//
//	Author: Michael Jordan
//	Description:
//		Updates the current position of the camera.
//		
//	Param:
//		- glm::vec3	| new camera position (unscaled).
//
//	Return: n/a		|
//
void Camera::SetPosition(glm::vec3 position)
{
	m_view.eye = position;

	CalculateViewMatrix();
}