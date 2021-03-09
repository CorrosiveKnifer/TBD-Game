#ifndef _ENTITY_CAMERA_H_
#define _ENTITY_CAMERA_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	EntityCamera.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	EntityCamera
// Description:
//		Camera extended class which is specifically follows a single entity subclass class.
//

//Parent Include
#include "Camera.h"

//Local Include
#include "Entity.h"

class EntityCamera :   public Camera
{
	//Member Functions:
public:
	EntityCamera(CamType type);
	virtual ~EntityCamera();

	virtual void Process(float dt);

	void SetEntity(Entity* e) { m_pTarget = e; if(m_pTarget != 0) m_targetPos = e->GetTransform().GetPosition(); };
	void SetFollowing(bool _following) { m_isFollowing = _following; };
protected:

private:
	//Member Data:
public:

protected:
	bool m_isFollowing; //Tracking enables following

	Entity* m_pTarget;
	
private:
	
};


#endif // !_FREE_CAMERA_H_