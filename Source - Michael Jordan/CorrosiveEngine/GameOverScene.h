#ifndef _GAME_OVER_SCENE_H_
#define _GAME_OVER_SCENE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	GameOverScene.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	GameOverScene
// Description:
//		Final Scene which will show once the game is over, indicating a loss or win.
//

//Parent Include
#include "SceneADT.h"

//Forward Declaration
class Button;
class Camera;
class CubeMap;
class TextGenerator;
class Program;

class GameOverScene :	 public SceneADT
{
	//Member Functions
public:
	GameOverScene(bool _isWin, unsigned int kills, double time, float accuracy);
	virtual ~GameOverScene();

	virtual bool Initialise();
	virtual void PreLoad();

	virtual void Draw();
	virtual void Process(float dT);

	virtual void OnMousePositionChange();
	virtual void OnMouseButtonChange();
	virtual void OnKeyChange() {};
	virtual void OnSpecialChange() {};
	virtual void Resize();
protected:

private:
	//Member Data:
public:

protected:
	bool m_isWin;
	unsigned int m_kills;
	double m_gameTime;
	float m_accuracy;
	float m_timeElapsed;

	Program* m_program;
	Camera* m_pMainCam;
	CubeMap* m_pCubeMap;
	TextGenerator* m_textGen60;
	Button* m_pBackButton;
private:

};

#endif