#ifndef _WIDGET_H_
#define _WIDGET_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Widget.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Widget
// Description:
//		Parent class to all screen projected widgets.
//

#define ALIGN_TOP_LEFT		glm::vec2(0.0f, 1.0f)
#define ALIGN_TOP_CENTRE	glm::vec2(0.5f, 1.0f)
#define ALIGN_TOP_RIGHT		glm::vec2(1.0f, 1.0f)
#define ALIGN_CENTRE_LEFT	glm::vec2(0.0f, 0.5f)
#define ALIGN_CENTRE		glm::vec2(0.5f, 0.5f)
#define ALIGN_CENTRE_RIGHT	glm::vec2(1.0f, 0.5f)
#define ALIGN_BOTTEM_LEFT	glm::vec2(0.0f, 0.0f)
#define ALIGN_BOTTEM_CENTRE	glm::vec2(0.5f, 0.0f)
#define ALIGN_BOTTEM_RIGHT	glm::vec2(1.0f, 0.0f)

//Local Includes
#include "Texture.h"
#include "Camera.h"
#include "Program.h"

#include <functional>

class Widget
{
	//Member Functions:
public:
	Widget();
	virtual ~Widget();

	virtual void Draw(Program* _program) = 0;
	virtual void Process(float dT) = 0;

	virtual void SetLocalPosition(glm::vec2 position) { m_position = position; };
	virtual void SetLocalScale(glm::vec2 scale) { m_scale = scale; };
	
	void SetLocalAlignment(glm::vec2 _align) { m_alignLocal = _align; };
	void SetCanvasAlignment(glm::vec2 _align) { m_alignCanvas = _align; };
	glm::vec2 GetLocalPosition() { return m_position; };
	glm::vec2 GetLocalScale() { return m_scale; };

	void SetMouseClickCallBack(std::function<void()> _callBack) { m_mouseClickCallBack = _callBack; };
	//virtual bool HandleMouseHover(glm::vec2 _mouse) = 0;
	//virtual bool HandleMouseClick(glm::vec2 _mouse) = 0;

	float GetValue() { return m_value; };
	void SetValue(float val) { m_value = val; };
	
protected:
	void DrawTexture(Program* _program, Texture& texture, glm::vec2 scale = glm::vec2(1.0f, 1.0f));

	virtual bool CheckMousePosition(glm::vec2 _mouse) = 0;
private:

	//Member Data:
public:

protected:
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;

	float m_value;
	glm::vec2 m_alignLocal;
	glm::vec2 m_alignCanvas;

	std::function<void()> m_mouseClickCallBack;
private:

	//Local
	glm::vec2 m_position;
	glm::vec2 m_scale;
	
};

#endif