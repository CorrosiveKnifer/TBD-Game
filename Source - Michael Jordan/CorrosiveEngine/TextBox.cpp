//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	TextBox.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	TextBox
// Description:
//		TextBox widget which accepts information typed into the keyboard.
//		Widget 'value' indicates if the current textbox is being focused.
//

//Self Include
#include "TextBox.h"

//Local Include
#include "InputHandler.h"
#include "TextGenerator.h"

//Constructor
TextBox::TextBox(float _align)
{
	m_align = _align;
	m_pBase = &Renderer::GetInstance().CreateTexture("TextBase.png");
	m_text = "";
	m_textGen15 = &Renderer::GetInstance().CreateTextGenerator("Forward.ttf", 15);
	InputHandler::GetInstance().SetTrackKeys(true);
}

//Destructor
TextBox::~TextBox()
{
	//Text generator deleted in renderer.
	m_textGen15 = 0;
}

//	Draw( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Draws this textBox onto the scene.
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void TextBox::Draw(Program* _program)
{
	m_textGen15->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	
	std::string display = m_text;
	if (m_value == 1.0f && m_timer > 0.8f)
	{
		display += '|';
	}

	glm::vec2 scale = GetLocalScale();
	float sizeX = m_textGen15->GetDimentionsOfText(m_text).x * 0.5f;

	glm::vec2 topLeft = glm::vec2(
		(m_alignCanvas.x + GetLocalPosition().x) - (m_alignLocal.x * m_pBase->GetSize().x),
		(m_alignCanvas.y + GetLocalPosition().y) - ((1.0f - m_alignLocal.y) * m_pBase->GetSize().y)
	);

	float posX = topLeft.x + m_align * ((m_pBase->GetSize().x * 0.5f * scale.x) - sizeX) + 7.5f;
	float posY = topLeft.y + (m_pBase->GetSize().y * 0.5f * scale.y) - 7.5f;

	while(m_pBase->GetSize().x - 18.0f < m_textGen15->GetDimentionsOfText(display).x)
	{
		display = display.substr(1);
	}

	m_textGen15->RenderText(display, glm::vec2(posX, posY));

	DrawTexture(_program, *m_pBase, GetLocalScale());
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes this textbox since the last call.
//		
//	Param:
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void TextBox::Process(float dT)
{
	if (InputHandler::GetInstance().IsMousePressed(GLUT_LEFT_BUTTON))
	{
		if (CheckMousePosition(InputHandler::GetInstance().GetMousePos()))
		{
			m_isFocused = true;
			
			//SetKeyTrace(true);
		}
		else
		{
			m_isFocused = false;
			//SetKeyTrace(false);
		}
	}
	else
		//SetKeyTrace(true); //Re-enable key trace iff focused.

	m_timer += dT;
	if (m_isFocused)
	{
		m_text = InputHandler::GetInstance().GetKeyTrace();
	}

	if (m_timer > 1.6f) //Flickering '|' to indicate it being in focus
	{
		m_timer = 0.0f;
	}
}

//	HandleMouseHover( _mouse )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Handles the button and returns if the mouse position is on top of
//		this textBox... in a hover case.
//		
//	Param:
//		- glm::vec2		|	Mouse position directly from glut
//
//	Return: n/a		|
//
bool TextBox::HandleMouseHover(glm::vec2 _mouse)
{
	if ((CheckMousePosition(_mouse)))
	{
		return true;
	}
	return false;
}

//	HandleMouseHover( _mouse )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Handles the button and returns if the mouse position is on top of
//		this textbox... in a click case (setting focus).
//		
//	Param:
//		- glm::vec2		|	Mouse position directly from glut
//
//	Return: n/a		|
//
bool TextBox::HandleMouseClick(glm::vec2 _mouse)
{
	if ((CheckMousePosition(_mouse)))
	{
		m_value = 1.0f;
		InputHandler::GetInstance().SetTrackKeys(true);
		InputHandler::GetInstance().SetCurrentTrace(m_text);
		return true;
	}

	m_value = 0.0f;
	InputHandler::GetInstance().SetTrackKeys(false);
	InputHandler::GetInstance().ClearKeyTrace();
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
bool TextBox::CheckMousePosition(glm::vec2 _mouse)
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

void TextBox::SetKeyTrace(bool _enable)
{
	if (_enable)
	{
		if (m_isFocused)
		{
			InputHandler::GetInstance().SetTrackKeys(true);
			InputHandler::GetInstance().SetCurrentTrace(m_text);
		}
	}
	else
	{
		InputHandler::GetInstance().SetTrackKeys(false);
		InputHandler::GetInstance().ClearKeyTrace();
	}
	
}
