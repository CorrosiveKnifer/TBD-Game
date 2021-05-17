#ifndef _SCENE_MANAGER_H_
#define _SCENE_MANAGER_H_

// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// © 2021 Media Design School
//
// Filename: SceneManager.h
// Description: Singleton class incharge of handling the scene transitions.
// Author(s): Michael Jordan
// 

#include <SFML/Graphics.hpp>
#include "Entity.h"
class Scene;

class SceneManager
{
	//Member function
public:
	static SceneManager& GetInstance();
	static void DestroyInstance();

	void DestroyEntity(Entity* entity);
	void DoSceneLoop();

	void TransitionTo(Scene* _nextScene) { m_nextScene = _nextScene; };
	void Quit() { m_isRunning = false; };
private:
	SceneManager();
	~SceneManager();

	void LoadNextScene();
protected:

	//Member data
public:

private:
	static SceneManager* sm_pInstance;
	static const unsigned int sm_totalFrames = 60;

	sf::Clock m_clock;
	bool m_isRunning;

	Scene* m_topScene;
	Scene* m_nextScene;
protected:

};

#endif //_WINDOW_H_