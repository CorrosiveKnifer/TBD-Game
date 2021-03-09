#ifndef _COMPONENT_H_
#define _COMPONENT_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Component.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Component
// Description:
//		A component is a piece of an entity which does not contain local/world positional data.
//

#include "Camera.h"
#include "Transform.h"

class Component
{
	//Member Functions
public:
	Component() {};
	~Component() {};

	virtual void Draw(Camera* cam, Program* program, Transform transform) const = 0;
	virtual void Process(float dt) = 0;

protected:
	
private:
	//Member Data
public:

protected:

private:

};

#endif //_COMPONENT_H_