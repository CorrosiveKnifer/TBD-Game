#ifndef _PAUSE_SCENE_H_
#define _PAUSE_SCENE_H_

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