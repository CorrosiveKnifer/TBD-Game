//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	InputHandler.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	InputHandler
// Description:
//		A singleton class in charge of tracking the current state of each key 
//		on the standard keyboard and the mouse buttons/position.
//

//Parent Include
#include "InputHandler.h"

//Static Variable
InputHandler* InputHandler::sm_pInstance = 0;

//Dependeny Includes
#include <freeglut.h>

//Default Constructor
InputHandler::InputHandler()
{
	m_mousePos = glm::vec2(0, 0);

	m_isFixedMouse = false;
	m_isTrackingKeys = false;
	m_keyTrace = "";
}

//Destructor
InputHandler::~InputHandler()
{

}

//	GetInstance( )
//
//	Author: Michael Jordan
//	Description:
//		Get function for the only instance of this class. If the instance doesn't
//		exist then it will make one.
//		
//	Param:
//		- n/a
//
//	Return: InputHandler&	|	Single instance of the InputHandler
//
InputHandler& InputHandler::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new InputHandler();
	}
	return *sm_pInstance;
}

//	DestroyInstance( )
//
//	Author: Michael Jordan
//	Description:
//		Destroys the single instance of this class
//		
//	Param:
//		- n/a
//
//	Return: n/a		|
//
void InputHandler::DestroyInstance()
{
	if (sm_pInstance != 0)
	{
		delete sm_pInstance;
		sm_pInstance = 0;
	}
}

void InputHandler::Process()
{
	m_mouseDelta = glm::vec2(0.0f, 0.0f); //reset Delta

	for (unsigned int i = 0; i < 12; i++)
	{
		m_prevSpecialFunc[i] = m_currSpecialFunc[i];
	}
	for (unsigned int i = 0; i < 4; i++)
	{
		m_prevSpecialArrow[i] = m_currSpecialArrow[i];
	}
	for (unsigned int i = 0; i < 255; i++)
	{
		m_prevKeyboard[i] = m_currKeyboard[i];
	}
	for (unsigned int i = 0; i < 3; i++)
	{
		m_prevMouse[i] = m_currMouse[i];
	}
}

//	OnKeyPressDown( key, x, y)
//
//	Author: Michael Jordan
//	Description:
//		Updates the key pressed to a down state.
//		Note: character 'a' is considered different to 'A'.
//		
//	Param:
//		- unsigned char |	key being pressed down.
//
//	Return: n/a		|
//
void InputHandler::OnKeyPressDown(unsigned char key)
{
	m_currKeyboard[key] = true;
}

//	OnKeyReleased( key, x, y)
//
//	Author: Michael Jordan
//	Description:
//		Updates the key released to a down state.
//		Note: character 'a' is considered different to 'A'.
//		
//	Param:
//		- unsigned char |	key being pressed down.
//
//	Return: n/a		|
//
void InputHandler::OnKeyReleased(unsigned char key)
{
	m_currKeyboard[key] = false;
}

//	OnSpecialPressed(key)
//
//	Author: Michael Jordan
//	Description:
//		Updates the status of the spcial button pressed.
//		Note: Only supports F1 to F12 and the arrow buttons.
//		
//	Param:
//		- int		|	special key being pressed.
//		
//	Return: n/a		|	
//
void InputHandler::OnSpecialPressed(int key)
{
	bool* specLoc = GetCurrentSpecialLoc(key);

	if (specLoc != 0)
	{
		*specLoc = true;
	}
}

//	OnSpecialReleased(key)
//
//	Author: Michael Jordan
//	Description:
//		Updates the status of the spcial button released.
//		Note: Only supports F1 to F12 and the arrow buttons.
//		
//	Param:
//		- int		|	special key being updated.
//		
//	Return: n/a		|	
//
void InputHandler::OnSpecialReleased(int key)
{
	bool* specLoc = GetCurrentSpecialLoc(key);

	if (specLoc != 0)
	{
		*specLoc = false;
	}
}

//	OnMouseClick( button, state, x, y)
//
//	Author: Michael Jordan
//	Description:
//		Updates the mouse button to a down state.
//		
//	Param:
//		- int		|	Button used on the mouse.
//		- state		|	Current status of the button.
//
//	Return: n/a		|
//
void InputHandler::OnMouseClick(int button, int state)
{
	if (button >= 3)
		return; //Invalid

	m_currMouse[button] = (state == GLUT_DOWN);
}

//	OnMouseMotion(x, y)
//
//	Author: Michael Jordan
//	Description:
//		Updates the position of the mouse when it is moved.
//		
//	Param:
//		- int		|	x coordinate of the mouse location.
//		- int		|	y coordinate of the mouse location.
//		
//	Return: n/a		|	
//
void InputHandler::OnMouseMotion(int x, int y)
{
	m_mouseDelta = glm::vec2(x, y) - m_mousePos;

	if (m_isFixedMouse)
	{
		m_mousePos = m_fixedPos;
		glutWarpPointer(static_cast<int>(m_fixedPos.x), static_cast<int>(m_fixedPos.y));
	}
	else
	{
		m_mousePos = glm::vec2(x, y);
	}
}

//	SetFixedMouse( _isFixed )
//
//	Author: Michael Jordan
//	Description:
//		Enables and disables the fixed state of the mouse (also sets the mouse invisible).
//		
//	Param:
//		- bool		|	new status of the mouse.
//		
//	Return: n/a		|	
//
void InputHandler::SetFixedMouse(bool _isFixed)
{
	if (_isFixed)
	{
		m_mousePos = m_fixedPos;
		glutWarpPointer(static_cast<int>(m_fixedPos.x), static_cast<int>(m_fixedPos.y));
		//glutSetCursor(GLUT_CURSOR_NONE);
	}
	else
	{
		glutSetCursor(GLUT_CURSOR_INHERIT);
	}
	m_isFixedMouse = _isFixed;
}

//	GetCurrentSpecialLoc(key)
//
//	Author: Michael Jordan
//	Description:
//		Gets the address associated with the special key current state.
//		
//	Param:
//		- int		|	spcial key being checked.
//		
//	Return: ButtonState*	|	Address of the special key's status.
//
bool* InputHandler::GetCurrentSpecialLoc(int key)
{
	if (key >= GLUT_KEY_F1 && key <= GLUT_KEY_F12)
	{
		return &m_currSpecialFunc[key];

	}
	switch (key)
	{
	case GLUT_KEY_LEFT:
		return &m_currSpecialArrow[0];

	case GLUT_KEY_RIGHT:
		return &m_currSpecialArrow[1];

	case GLUT_KEY_UP:
		return &m_currSpecialArrow[2];

	case GLUT_KEY_DOWN:
		return &m_currSpecialArrow[3];

	default:
		//Rest is not supported.
		break;
	}
	return 0;
}

//	GetPreviousSpecialLoc(key)
//
//	Author: Michael Jordan
//	Description:
//		Gets the address associated with the special key previous state.
//		
//	Param:
//		- int		|	spcial key being checked.
//		
//	Return: ButtonState*	|	Address of the special key's status.
//
bool* InputHandler::GetPreviousSpecialLoc(int key)
{
	if (key >= GLUT_KEY_F1 && key <= GLUT_KEY_F12)
	{
		return &m_prevSpecialFunc[key];

	}
	switch (key)
	{
	case GLUT_KEY_LEFT:
		return &m_prevSpecialArrow[0];

	case GLUT_KEY_RIGHT:
		return &m_prevSpecialArrow[1];

	case GLUT_KEY_UP:
		return &m_prevSpecialArrow[2];

	case GLUT_KEY_DOWN:
		return &m_prevSpecialArrow[3];

	default:
		//Rest is not supported.
		break;
	}
	return 0;
}

//	AddToTrace(key)
//
//	Author: Michael Jordan
//	Description:
//		Adds a key to the trace with additional support for backspace, which will
//		instead remove the last key, and the enter (stops tracing). 
//		
//	Param:
//		- unsigned char		|	Key to add to the trace.
//		
//	Return: n/a		|
//
void InputHandler::AddToTrace(unsigned char key)
{
	if (key >= 32 && key <= 126) //Characters
	{
		m_keyTrace += key;
		return;
	}

	if (key == 8) //Backspace
	{
		m_keyTrace = m_keyTrace.substr(0, m_keyTrace.size() - 1);
		return;
	}

	if (key == 13) //Enter
	{
		m_isTrackingKeys = false;
		return;
	}
}
