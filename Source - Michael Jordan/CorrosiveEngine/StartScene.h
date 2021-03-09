#ifndef _START_SCENE_H_
#define _START_SCENE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	StartScene.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	StartScene
// Description:
//		Starting scene on a timer, to display the engine logo.
//

//Parent Include
#include "SceneADT.h"
#include "LightingSystem.h"

#include "MeshEntity.h"
class Program;

class StartScene :	public SceneADT
{
	//Member Functions:
public:
	StartScene();
	virtual ~StartScene();

	virtual bool Initialise();
	virtual void PreLoad();

	virtual void Draw();
	virtual void Process(float dT);
	virtual void Resize();
protected:

private:

	//Member Data:
public:

protected:
	MeshEntity* m_logo;
	PointLight* m_light;

	float m_logoAlpha;
private:

};

#endif // !_START_SCENE_H_