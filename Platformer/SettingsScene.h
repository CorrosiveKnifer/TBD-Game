#ifndef _SettingsScene_H_
#define _SettingsScene_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: SettingsScene.h
// Description   	: Scene for the settings menu with volume controls.
// Author         	: Bailey Johnson
// Mail         	: bailey.johnson@mds.ac.nz
//
//Parent Include
#include "Scene.h"

//Forward Declaration
class Button;

class SettingsScene : public Scene
{
public:
	SettingsScene();
	virtual ~SettingsScene();

	//Inherited functions:
	virtual void Draw();
	virtual void Update(float dT);
	virtual void DestroyEntity(Entity* entity) {};

	//Button functions:
	void volumeUp();
	void volumeDown();
	void Back();
private:

protected:
	//Member data
public:

private:

protected:
	sf::Vector2f m_mousePos;
	sf::Vector2f* m_mousePressPos;

	sf::Texture menuBackgroundTex;
	sf::Sprite menuBackgroundSpr;

	Button* m_pVolumeUpBtn;
	Button* m_pVolumeDownBtn;
	Button* m_pBackBtn;

	bool m_hasMouseClicked;

	float m_pVolume = 100.0f;

	sf::Texture masterVolumeTex;
	sf::Sprite masterVolumeSpr;

	int menuSelection = 1;

	//Test variable
	int keypressed;
};
#endif

