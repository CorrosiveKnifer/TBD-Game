#ifndef _SETTINGS_MENU_SCENE_H_
#define _SETTINGS_MENU_SCENE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	SettingsMenuScene.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	SettingsMenuScene
// Description:
//		Settings scene to alter the global setting like volume.
//

//Parent Include
#include "SceneADT.h"

//Forward Declaration
class Mesh;
class Button;
class TextGenerator;
class ScaleBar;
class CubeMap;
class Camera;
class Canvas;
class Program;

class SettingsMenuScene :  public SceneADT
{
	//Member Functions:
public:
	SettingsMenuScene(bool fromNetwork = false);
	virtual ~SettingsMenuScene();

	virtual bool Initialise();
	virtual void PreLoad();

	virtual void Draw();
	virtual void Process(float dT);
	virtual void Resize();

	void Back();
protected:

private:

	//Member Data:	
public:

protected:
	Program* m_program;
	float m_timeElapsed;

	TextGenerator* m_textGen35;
	TextGenerator* m_textGen15;

	//Button* m_pBackButton;
	//ScaleBar* m_BGBar;
	//ScaleBar* m_SEBar;

	Canvas* m_pCanvas;
	CubeMap* m_pCubeMap;

	Camera* m_pMainCam;
	
	bool m_fromNetwork;
	bool m_hasReturned;
private:

};

#endif //_SETTINGS_MENU_SCENE_H_