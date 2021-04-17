#ifndef _LogoScene_H_
#define _LogoScene_H_

// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// © 2021 Media Design School
//
// Filename: LogoScene.hpp
// Description: Singleton class incharge of handling the scene transitions.
// Author(s): Michael Jordan
// 
//Parent Include
#include "Scene.h"

//Library Includes
#include <vector>

//Implementation
class LogoScene : public Scene
{
	//Member function
public:
	LogoScene();
	virtual ~LogoScene();

	//bool Initialise(BackBuffer* _backBuffer, SoundBuffer* _soundBuffer);
	//bool Deinitialise();

	//Inheritance function:
	//virtual bool DoWindowLoop();
	virtual void Draw();
	virtual void Update(float dT);

	void SetInputEnabled(bool _input);
	void SetLogoLoc(sf::Vector2f logoLoc);
	void SetLogo(sf::Sprite* sp);
	void SetBackgroundColour(sf::Color col);
private:

protected:

	//Member data
public:

private:

protected:
	sf::Sprite* m_logo;
	sf::Color m_screenColor;
	bool m_isInputEnabled;

	float m_timePassed;
	float m_renderTime;

	bool m_hasFinished;
};
#endif //_LogoScene_H_