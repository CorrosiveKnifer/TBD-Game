//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Button.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Button
// Description:
//		Button to interact with.
//

//Self Include
#include "Button.h"
#include "InputHandler.h"

//Constructor
Button::Button(std::string _faceText, std::string _baseTexture)
{
	m_pBase = &Renderer::GetInstance().CreateTexture(_baseTexture.c_str());

	m_label = new Label(_faceText, "Forward.ttf", 15);
	m_label->SetColour(m_standardColor);
	m_label->SetLocalAlignment(ALIGN_CENTRE);

	m_value = 0.0f;
	m_isEnabled = true;
	m_isHovered = false;
}

//Destructor
Button::~Button()
{
	//Base texture is deleted in widget.
	//Text Generator is deleted in renderer.
	//m_textGen15 = 0;

	delete m_label;
	m_label = 0;
}

//	Draw( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Draws this button onto the scene.
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void Button::Draw(Program* _program)
{
	glm::vec2 scale = GetLocalScale();
	//float sizeX = m_textGen15->GetDimentionsOfText(m_text).x * 0.5f;

	glm::vec2 topLeft = glm::vec2(
		(m_alignCanvas.x + GetLocalPosition().x) - (m_alignLocal.x * m_pBase->GetSize().x),
		(m_alignCanvas.y + GetLocalPosition().y) - ((1.0f - m_alignLocal.y) * m_pBase->GetSize().y)
	);

	float posX = topLeft.x + (m_pBase->GetSize().x * 0.5f * scale.x);
	float posY = topLeft.y + (m_pBase->GetSize().y * 0.5f * scale.y);

	m_label->SetCanvasAlignment(glm::vec2(posX, posY));
	m_label->Draw(_program);

	DrawTexture(_program, *m_pBase, GetLocalScale());
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes this button since the last call.
//		
//	Param:
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void Button::Process(float dT)
{
	if (!m_isEnabled) 
	{
		m_label->SetColour(m_disableColor);
		return;
	}
		
	if (CheckMousePosition(InputHandler::GetInstance().GetMousePos()))
	{
		if (InputHandler::GetInstance().IsMousePressed(GLUT_LEFT_BUTTON))
		{
			m_label->SetColour(m_standardColor);
			m_value = 1.0f;
			if(m_mouseClickCallBack != NULL)
				m_mouseClickCallBack();
			return;
		}
		else
		{
			m_label->SetColour(m_hoverColor);
			m_value = 0.0f;
			return;
		}
	}
	else
	{
		m_label->SetColour(m_standardColor);
		m_value = 0.0f;
		return;
	}
}

//	HandleMouseHover( _mouse )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Handles the button and returns if the mouse position is on top of
//		this button... in a hover case.
//		
//	Param:
//		- glm::vec2		|	Mouse position directly from glut
//
//	Return: n/a		|
//
bool Button::HandleMouseHover(glm::vec2 _mouse)
{
	m_isHovered = false;

	if (!m_isEnabled)
		return false;

	if ((CheckMousePosition(_mouse)))
	{
		m_isHovered = true;
		return true;
	}
	
	return false;
}

//	HandleMouseClick( _mouse )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Handles the button and returns if the mouse position is on top of
//		this button... in a click case.
//		
//	Param:
//		- glm::vec2		|	Mouse position directly from glut
//
//	Return: n/a		|
//
bool Button::HandleMouseClick(glm::vec2 _mouse)
{
	if (!m_isEnabled)
		return false;

	return CheckMousePosition(_mouse);
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
bool Button::CheckMousePosition(glm::vec2 _mouse)
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