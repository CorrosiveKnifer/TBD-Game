//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	FreeCamera.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	FreeCamera
// Description:
//		Camera extended to add a roaming functionality.
//

//Self Include
#include "FreeCamera.h"

//Local Include
#include "InputHandler.h"

//Constructor
FreeCamera::FreeCamera(CamType type)
{
	m_type = type;
		
	m_worldScale = glm::vec3(1.0f, 1.0f, 1.0f);
	m_camDirect = glm::vec4(glm::normalize(m_view.at - m_view.eye), 1.0f);

	SetUpDefault();
}

//Destructor
FreeCamera::~FreeCamera()
{

}

void FreeCamera::Activate()
{
	InputHandler::GetInstance().SetFixedMouse(true);
	InputHandler::GetInstance().SetFixedMousePos(glm::vec2(Renderer::SCREEN_WIDTH / 2, Renderer::SCREEN_HEIGHT / 2));
	m_isActive = true;
}

void FreeCamera::Deactivate()
{
	InputHandler::GetInstance().SetFixedMouse(false);
	m_isActive = false;
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Calculates any input changes to update the camera position and look.
//		
//	Param:
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void FreeCamera::Process(float dt)
{
	glm::vec2 mouseDelta = InputHandler::GetInstance().GetMouseDelta();

	if(mouseDelta.x != 0.0f)
		mouseDelta.x = (mouseDelta.x > 0) ? 1.0f : -1.0f;

	if (mouseDelta.y != 0.0f)
		mouseDelta.y = (mouseDelta.y > 0) ? 1.0f : -1.0f;

	//Update move speed
	if (InputHandler::GetInstance().IsKeyPressed('+')) //Speed Up
	{
		m_moveSpeed++;
	}
	if (InputHandler::GetInstance().IsKeyPressed('-')) //Speed Down
	{
		if(m_moveSpeed > 0)
			m_moveSpeed--;
	}

	//Rotation caused by the mouse movement
	float lookDist = m_lookSpeed * dt;
	float yaw = -mouseDelta.x * lookDist;
	float pitch = (m_camDirect.z < 0) ? -mouseDelta.y * lookDist : mouseDelta.y * lookDist;

	//Distance in this update
	float dist = m_moveSpeed * dt;

	//Default input before any key presses
	glm::vec3 netPersp; //Perspective change (3D)
	glm::vec2 netOrtho; //Orthographic change (2D)

	//Rotate the camera
	glm::mat4 rotation;
	rotation = glm::rotate(rotation, pitch, glm::vec3(1.0f, 0.0f, 0.0f));
	rotation = glm::rotate(rotation, yaw, glm::vec3(0.0f, 1.0f, 0.0f));
	rotation = glm::rotate(rotation, 0.0f, glm::vec3(0.0f, 0.0f, 1.0f));

	glm::vec4 newAt = rotation * glm::vec4(glm::normalize(m_view.at - m_view.eye), 1.0f);
	m_view.at = m_view.eye + glm::vec3(newAt);

	//Movement input:
	if (m_isActive)
	{
		m_camDirect = glm::vec4(glm::normalize(m_view.at - m_view.eye), 1.0f);
		if (InputHandler::GetInstance().IsKeyPressed('a') || InputHandler::GetInstance().IsKeyPressed('A')) //CameraLeft
		{
			netPersp -= glm::normalize(glm::cross(glm::vec3(m_camDirect), m_view.up)) * dist;
			netOrtho.x -= dist;
		}
		if (InputHandler::GetInstance().IsKeyPressed('d') || InputHandler::GetInstance().IsKeyPressed('D')) //CameraRight
		{
			netPersp += glm::normalize(glm::cross(glm::vec3(m_camDirect), m_view.up)) * dist;
			netOrtho.x += dist;
		}
		if (InputHandler::GetInstance().IsKeyPressed('w') || InputHandler::GetInstance().IsKeyPressed('W')) //CameraForward
		{
			netPersp += glm::vec3(m_camDirect) * dist;
			netOrtho.y += dist;
		}
		if (InputHandler::GetInstance().IsKeyPressed('s') || InputHandler::GetInstance().IsKeyPressed('S')) //CameraBackwards
		{
			netPersp -= glm::vec3(m_camDirect) * dist;
			netOrtho.y -= dist;
		}

		switch (m_type) //Update matrixes based on type.
		{
		default:
		case Camera::CamType::Perspective:

			//Change in movement
			m_view.eye += netPersp;
			m_view.at += netPersp;

			//Remake look At:
			CalculateViewMatrix();
			break;

		case Camera::CamType::Orthographic:
			m_view.eye += glm::vec3(netOrtho, 0.0f);
			m_view.at += glm::vec3(netOrtho, 0.0f);;

			CalculateViewMatrix();
			break;
		}
	}
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
void FreeCamera::RemakeMatrix()
{
	InputHandler::GetInstance().SetFixedMousePos(glm::vec2(Renderer::SCREEN_WIDTH / 2, Renderer::SCREEN_HEIGHT / 2));
	Camera::RemakeMatrix();
}
