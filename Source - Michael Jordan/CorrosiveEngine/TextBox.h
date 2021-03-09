#ifndef _TEXTBOX_H_
#define _TEXTBOX_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	TextBox.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	TextBox
// Description:
//		TextBox widget which accepts information typed into the keyboard.
//		Widget 'value' indicates if the current textbox is being focused.
//

//Parent Include
#include "Widget.h"

//Forward Declaration
class TextGenerator;

class TextBox :   public Widget
{
	//Member Functions:
public:
	TextBox(float _align = 0.0f);
	virtual ~TextBox();

	virtual void Draw(Program* _program);
	virtual void Process(float dT);

	virtual bool HandleMouseHover(glm::vec2 _mouse);
	virtual bool HandleMouseClick(glm::vec2 _mouse);

	std::string GetText() { return m_text; };
	void SetText(std::string _text) { m_text = _text; };

protected:
	virtual bool CheckMousePosition(glm::vec2 _mouse);

	void SetKeyTrace(bool _enable);
private:

	//Member Data:
public:

protected:
	Texture* m_pBase;
	std::string m_text;

	bool m_isFocused;

	TextGenerator* m_textGen15;

	float m_timer;
	float m_align;
private:
};

#endif // !_TEXTBOX_H_