//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	ScaleBar.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	ScaleBar
// Description:
//		Scale bar handles a range of values on mouse click.
//

//Self include
#include "ScaleBar.h"

#include "InputHandler.h"

//Default Constructor
ScaleBar::ScaleBar()
{
	m_pTop = 0;
	m_value = 1.0f;
	m_isFocused = false;
}

//Constructor
ScaleBar::ScaleBar(std::string _base, std::string _top)
{
	m_pBase = &Renderer::GetInstance().CreateTexture(_base.c_str());
	m_pTop = &Renderer::GetInstance().CreateTexture(_top.c_str());
	m_value = 1.0f;
	m_isFocused = false;
}

//Destructor
ScaleBar::~ScaleBar()
{
	//Textures deleted in renderer
	m_pTop = 0;
}

//	Draw( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Draws this scale bar onto the scene.
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void ScaleBar::Draw(Program* _program)
{
	float xScale = (m_value >= 0) ? m_value : 0;

	DrawTexture(_program, *m_pTop, glm::vec2(xScale, 1.0f));
	DrawTexture(_program, *m_pBase, glm::vec2(1.0f, 1.0f));
	//Widget::Draw();
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes this scalebar since the last call.
//		
//	Param:
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void ScaleBar::Process(float dT)
{
	glm::vec2 _mouse = InputHandler::GetInstance().GetMousePos();
	if (CheckMousePosition(_mouse) || m_isFocused)
	{
		if (InputHandler::GetInstance().IsMousePressed(GLUT_LEFT_BUTTON))
		{
			m_isFocused = true;

			float width = m_pBase->GetSize().x * GetLocalScale().x;
			float height = m_pBase->GetSize().y * GetLocalScale().y;

			glm::vec2 topLeft = glm::vec2(
				(m_alignCanvas.x + GetLocalPosition().x) - (m_alignLocal.x * width),
				(m_alignCanvas.y + GetLocalPosition().y) - ((1.0f - m_alignLocal.y) * height)
			);

			float dx = _mouse.x - topLeft.x;
			m_value = dx / m_pBase->GetSize().x;

			if (m_value > 1.0f)
				m_value = 1.0f;

			if (m_value < 0.0f)
				m_value = 0.0f;

			if (m_mouseClickCallBack != NULL)
				m_mouseClickCallBack();
		}
	}

	if (!InputHandler::GetInstance().IsMousePressed(GLUT_LEFT_BUTTON))
	{
		m_isFocused = false;
	}
}

//	HandleMouseHover( _mouse )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Handles the scalebar and returns if the mouse position is on top of
//		this button... in a hover case.
//		
//	Param:
//		- glm::vec2		|	Mouse position directly from glut
//
//	Return: n/a		|
//
bool ScaleBar::HandleMouseHover(glm::vec2 _mouse)
{
	//Do nothing.
	return CheckMousePosition(_mouse);
}

//	HandleMouseHover( _mouse )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Handles the scalebar and returns if the mouse position is on top of
//		this button... in a click case. Also sets the value to where the mouse is.
//		
//	Param:
//		- glm::vec2		|	Mouse position directly from glut
//
//	Return: n/a		|
//
bool ScaleBar::HandleMouseClick(glm::vec2 _mouse)
{
	if (CheckMousePosition(_mouse))
	{
		float dx =_mouse.x - GetLocalPosition().x;
		m_value = dx/m_pBase->GetSize().x;
		return true;
	}

	return false;
}

//	CheckMousePosition( _mouse )
//
//	Author: Michael Jordan
//	Access: protected
//	Description:
//		Tests if the mouse location is within this button.
//		
//	Param:
//		- glm::vec2		|	Mouse position directly from glut.	
//
//	Return: n/a		|
//
bool ScaleBar::CheckMousePosition(glm::vec2 _mouse)
{
	float mouseX = _mouse.x;
	float mouseY = Renderer::SCREEN_HEIGHT - _mouse.y;

	float width = m_pBase->GetSize().x * GetLocalScale().x;
	float height = m_pBase->GetSize().y * GetLocalScale().y;

	glm::vec2 topLeft = glm::vec2(
		(m_alignCanvas.x + GetLocalPosition().x) - (m_alignLocal.x * width),
		(m_alignCanvas.y + GetLocalPosition().y) - ((1.0f - m_alignLocal.y) * height)
	);

	bool xAxis = (mouseX > topLeft.x) && (mouseX < topLeft.x + width);
	bool yAxis = (mouseY > topLeft.y) && (mouseY < topLeft.y + height);

	return xAxis && yAxis;
}
