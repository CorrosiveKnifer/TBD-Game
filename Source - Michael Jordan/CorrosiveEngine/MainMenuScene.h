#ifndef _MAIN_MENU_SCENE_H_
#define _MAIN_MENU_SCENE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	MainMenuScene.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	MainMenuScene
// Description:
//		Main Menu scene to launch the core game loot or change settings.
//

//Parent Include
#include "SceneADT.h"

//Forward Declaration
class MeshEntity;
class Button;
class CubeMap;
class Camera;
class TextGenerator;
class Canvas;
class Program;

class MainMenuScene : public SceneADT
{
	//Member Functions:
public:
	MainMenuScene();
	virtual ~MainMenuScene();

	virtual bool Initialise();
	virtual void PreLoad();

	void SetTimeElapsed(float elapse) { m_timeElapsed = elapse; };

	virtual void Draw();
	virtual void Process(float dT);
	virtual void Resize();

	void Open3DTest();
	void Open2DTest();
	void Settings();
	void Quit();
protected:

private:

	//Member Data:
public:

protected:
	Program* m_program;
	MeshEntity* m_backOrb;

	float m_timeElapsed;

	bool m_showButtons;

	bool m_isUnloaded;
	Camera* m_pMainCam;
	CubeMap* m_pCubeMap;
	TextGenerator* m_Avara60;
	TextGenerator* m_Avara15;
	Canvas* m_pCanvas;

private:

};


#endif // !_MAIN_MENU_SCENE_H_