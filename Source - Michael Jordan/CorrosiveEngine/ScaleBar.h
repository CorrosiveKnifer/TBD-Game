#ifndef _SCALE_BAR_H_
#define _SCALE_BAR_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	ScaleBar.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	ScaleBar
// Description:
//		Scale bar handles a range of values on mouse click.
//

//Parent Include
#include "Widget.h"

class ScaleBar :  public Widget
{
	//Member Functions:
public:
	ScaleBar();
	ScaleBar(std::string _base, std::string _top);
	virtual ~ScaleBar();

	virtual void Draw(Program* _program); //To render object to the screen.
	virtual void Process(float dT); //To update the object.

	virtual bool HandleMouseHover(glm::vec2 _mouse);
	virtual bool HandleMouseClick(glm::vec2 _mouse);

	void SetValue(float _value) { m_value = _value; };

protected:
	virtual bool CheckMousePosition(glm::vec2 _mouse);
private:

	//Member Data:
public:

protected:

private:
	Texture* m_pBase;
	Texture* m_pTop;

	bool m_isFocused;
};

#endif //_SCALE_BAR_H_