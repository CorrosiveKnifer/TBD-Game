#ifndef _BUTTON_H_
#define _BUTTON_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name     	: Button.h
// Description   	: A button that temporarily toggles when clicked.
// Author         	: Michael Jordan
// Mail         	: michael.jor8834@mediadesign.com
//
//Parent Include
#include "Widget.h"

#include <functional>
//Forward Declaration
class SpriteMap;

//Implementation
class Button : public Widget
{
	//Member function
public:
	Button();
	virtual ~Button();

	bool Initialise(sf::Sprite* _sprite, std::function<void()> clickCallback);

	virtual void Draw();
	virtual void Update();
	virtual bool HandleMouse(sf::Vector2i pos);

private:

protected:
	//Member Data
public:

private:

protected:
	SpriteMap* m_Anim;
	std::function<void()> o_clickCallback;
};
#endif // _BUTTON_H_