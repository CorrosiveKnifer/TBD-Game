#ifndef _LOAD_SCREEN_SCENE_H_
#define _LOAD_SCREEN_SCENE_H_

#include "SceneADT.h"
#include "Canvas.h"

class LoadScreenScene :    public SceneADT
{
	//Member Functions:
public:
	LoadScreenScene(SceneADT* _newScene);
	virtual ~LoadScreenScene();

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
	Canvas* m_pCanvas;
	SceneADT* m_scene;

	unsigned int m_packetTotal;
private:

};

#endif // !_LOAD_SCREEN_SCENE_H_