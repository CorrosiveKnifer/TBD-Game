#ifndef _PAUSE_SCENE_H_
#define _PAUSE_SCENE_H_

// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: PauseMenuScene.h
// Description   	: Pause scene when a player presses the start key.
// Author         	: Michael Jordan, Bailey Johnson
// Mail         	: michael.jor8834@mediadesign.com, bailey.johnson@mds.ac.nz
//
#include "Scene.h"

class PauseMenuScene :	public Scene
{
	//Member function
public:
	PauseMenuScene();
	PauseMenuScene(Scene* underScene);
	virtual ~PauseMenuScene();

	virtual void Draw();
	virtual void Update(float dt);
	virtual void DestroyEntity(Entity* entity) {};

	Scene* GetUnderScene() { return m_underScene; }
	
	void BackToMainMenu();
private:

protected:
	//Member data
public:

private:

protected:
	Scene* m_underScene;
};


#endif // !_PAUSE_SCENE_H_