#ifndef _BUTTON_H
#define _BUTTON_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Button.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Button
// Description:
//		Button to interact with.
//

//Parent Include
#include "Widget.h"
#include "Label.h"

class Button :  public Widget
{
	//Member Functions:
public:
	Button(std::string _faceText, std::string _baseTexture);
	virtual ~Button();

	virtual void Draw(Program* _program);
	virtual void Process(float dT);

	virtual bool HandleMouseHover(glm::vec2 _mouse);
	virtual bool HandleMouseClick(glm::vec2 _mouse);

	void SetText(std::string _text) { m_label->SetText(_text); };
	void SetEnabled(bool _enable) { m_isEnabled = _enable; };

protected:
	virtual bool CheckMousePosition(glm::vec2 _mouse);
private:

	//Member Data:
public:

protected:
	Texture* m_pBase;

	Label* m_label;

	bool m_isEnabled;
	bool m_isHovered;

	const glm::vec3 m_standardColor = glm::vec3(1.0f, 1.0f, 1.0f);
	const glm::vec3 m_hoverColor = glm::vec3(0.8f, 0.8f, 0.0f);
	const glm::vec3 m_disableColor = glm::vec3(0.45f, 0.45f, 0.45f);
private:
	
};

#endif //_BUTTON_H_