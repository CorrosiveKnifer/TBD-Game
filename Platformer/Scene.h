#ifndef _SCENE_H_
#define _SCENE_H_

#include "Renderer.h"
#include "Entity.h"

enum SceneName {
	LEVEL1,
	LEVEL2,
	MAINMENU,
	PLAYERSELECT,
	CONTROLS,
	SETTINGS,
	LOGO,
	VICTORY,
};

class Scene
{
	//Member function
public:
	Scene() { o_pRenderer = &Renderer::GetInstance(); }
	virtual ~Scene() {}

	virtual void Draw() = 0;
	virtual void Update(float dt) = 0;
	virtual void DestroyEntity(Entity* entity) = 0;

	int sceneID = 0;
	bool buttonPressed = false;

	bool upPressed = false;
	bool downPressed = false;
	bool leftPressed = false;
	bool rightPressed = false;

private:

protected:
	//Member data
public:

private:

protected:
	Renderer* o_pRenderer;
};

#endif //_SCENE_H_