#ifndef _INPUTHANDLER_H_
#define _INPUTHANDLER_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	InputHandler.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	InputHandler
// Description:
//		A singleton class in charge of tracking the current state of each key 
//		on the standard keyboard and the mouse buttons/position.
//

//Local Includes
#include "Renderer.h"

class InputHandler
{
	//Member Functions:
public:

	//Enums
	//enum class ButtonState { BUTTON_UP, BUTTON_DOWN, BUTTON_DOWN_FIRST };

	static InputHandler& GetInstance();
	static void DestroyInstance();

	void Process();

	//Scene Manager Func interaction
	void OnKeyPressDown(unsigned char key);
	void OnKeyReleased(unsigned char key);
	void OnSpecialPressed(int key);
	void OnSpecialReleased(int key);
	void OnMouseClick(int button, int state);
	void OnMouseMotion(int x, int y);

	//Record key trace for text input
	void SetCurrentTrace(std::string _trace) { m_keyTrace = _trace; };
	void SetTrackKeys(bool _isTrack) { m_isTrackingKeys = _isTrack; };
	bool IsTracing() { return m_isTrackingKeys; };
	std::string GetKeyTrace() { return m_keyTrace; };
	void ClearKeyTrace() { m_keyTrace.clear(); };
	
	//Lock mouse to screen and start using deltaMousePos
	void SetFixedMouse(bool _isFixed);
	void SetFixedMousePos(glm::vec2 _pos) { m_fixedPos = _pos; };

	//Get Key Information
	const bool IsKeyReleased(unsigned char key) { return (!m_currKeyboard[key]); };
	const bool IsKeyReleasedFirst(unsigned char key) { return (!m_currKeyboard[key]) && (m_prevKeyboard[key]); };
	const bool IsKeyPressed(unsigned char key, bool isCase = true) { return m_currKeyboard[key]; };
	const bool IsKeyPressedFirst(unsigned char key) { return (m_currKeyboard[key]) && (!m_prevKeyboard[key]); };

	const bool IsSpecialReleased(int key) { return (!GetCurrentSpecialLoc(key)); };
	const bool IsSpecialReleasedFirst(int key) { return (!GetCurrentSpecialLoc(key)) && (GetPreviousSpecialLoc(key)); };
	const bool IsSpecialPressed(int key) { return GetCurrentSpecialLoc(key); };
	const bool IsSpecialPressedFirst(int key) { return (GetCurrentSpecialLoc(key)) && (!GetPreviousSpecialLoc(key)); };

	const bool IsMouseReleased(int button) { return (!m_currMouse[button]); };
	const bool IsMouseReleasedFirst(int button) { return (!m_currMouse[button]) && (m_prevMouse[button]); };
	const bool IsMousePressed(int button) { return m_currMouse[button]; };
	const bool IsMousePressedFirst(int button) { return (m_currMouse[button]) && (!m_prevMouse[button]); };
	

	const glm::vec2 GetMousePos() { return m_mousePos; };
	const glm::vec2 GetMouseDelta() { return m_mouseDelta; };
protected:
	bool* GetCurrentSpecialLoc(int key);
	bool* GetPreviousSpecialLoc(int key);

	void AddToTrace(unsigned char key);
private:
	InputHandler();
	~InputHandler();

	//Member Data:
public:

protected:

private:
	static InputHandler* sm_pInstance;

	bool m_isFixedMouse;
	bool m_isTrackingKeys;

	std::string m_keyTrace;

	glm::vec2 m_fixedPos;
	glm::vec2 m_mousePos;
	glm::vec2 m_mouseDelta;

	//currentFrame
	bool m_currSpecialFunc[12] = { false };
	bool m_currSpecialArrow[4] = { false };
	bool m_currKeyboard[255] = { false };
	bool m_currMouse[3] = { false };

	//prevFrame
	bool m_prevSpecialFunc[12] = { false };
	bool m_prevSpecialArrow[4] = { false };
	bool m_prevKeyboard[255] = { false };
	bool m_prevMouse[3] = { false };
};

#endif // !_INPUTHANDLER_H_