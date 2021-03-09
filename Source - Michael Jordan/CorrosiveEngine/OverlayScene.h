#ifndef _OVERLAY_SCENE_H_
#define _OVERLAY_SCENE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	OverlayScene.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	OverlayScene
// Description:
//		Overlay Scene which uses another scene as a render (simulating a pause screen).
//

//Parent Include
#include "SceneADT.h"

//Forward Declaration
class Button;
class TextGenerator;

class OverlayScene :   public SceneADT
{
    //Member Functions
public:
    OverlayScene(SceneADT* _belowScene, bool isPaused = true);
    ~OverlayScene();

	virtual void Draw();
	virtual void Process(float dT);

	virtual void OnMousePositionChange();
	virtual void OnMouseButtonChange();
	virtual void OnKeyChange();
	virtual void OnSpecialChange();
	virtual void Resize();

	SceneADT* GetBelowScene() { return m_belowScene; };
protected:

private:
    //Member Data:
public:

protected:
	bool m_isPaused;
	SceneADT* m_belowScene;
	TextGenerator* m_textGen60;

	Button* m_pResumeButton;
	Button* m_pSettingButton;
	Button* m_pMenuButton;

	
private:
};

#endif