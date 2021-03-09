#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	SceneManager.h
// Author(s)    |	SceneManager Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	SceneManager
// Description:
//		Scene Manager that contains static functions to call
//		SceneADT functions. This class also stores the Scene stack and is incharge of
//		switching/freeing scenes.
//

//Local Includes
#include "SceneADT.h"
#include "LoadScreenScene.h"

//Library Include
#include <stack>

class SceneManager
{
	//Static Functions:
public:
	//Stack management
	static void PushScene(SceneADT* topScene, bool _willInitialise = false);
	static SceneADT* PopScene(bool willDelete = true);
	static void InstantiateEntity(Entity* entity);
	static void DestroyEntity(Entity* entity, bool toDeconstruct = false);
	static void LoadScene(SceneADT* _newScene);
	static SceneADT* TopScene();
	static void EmptyScenes();

	//SceneADT functions:
	static void Draw();
	static void Process();
	static void OnMouseMove(int x, int y);
	static void OnMouseClick(int button, int state, int x, int y);
	static void OnKeyPressed(unsigned char key, int x, int y);
	static void OnKeyReleased(unsigned char key, int x, int y);
	static void OnSpecialPressed(int key, int x, int y);
	static void OnSpecialReleased(int key, int x, int y);
	static void Resize(int width, int height);

protected:

private:

	//Static Data:
public:

protected:
	static std::stack<SceneADT*> m_pScenesStack;
	static float sm_prevTime;
	static SceneADT* nextScene;

private:

};

#endif // !_GLMANAGER_H_