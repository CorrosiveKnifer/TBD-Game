#ifndef _FREE_CAMERA_H_
#define _FREE_CAMERA_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	FreeCamera.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	FreeCamera
// Description:
//		Camera extended to add a roaming functionality.
//

//Parent includes
#include "Camera.h"

class FreeCamera : public Camera
{
	//Member Functions:
public:
	FreeCamera(CamType type);
	virtual ~FreeCamera();

	void Activate();
	void Deactivate();

	virtual void Process(float dt);
	virtual void RemakeMatrix();

protected:

private:
	//Member Data:
public:

protected:

private:
	glm::vec4 m_camDirect;

	bool m_isActive;

	float m_moveSpeed = 5.0f;
	float m_lookSpeed = 3.0f;
};

#endif // !_FREE_CAMERA_H_
