//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Widget.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Widget
// Description:
//		Parent class to all screen projected widgets.
//

//Self Include
#include "Widget.h"

//Constructor
Widget::Widget()
{
	m_value = 0.0f;

	m_mouseClickCallBack = NULL;

	m_position = glm::vec2(0.0f, 0.0f);
	m_scale = glm::vec2(1.0f, 1.0f);

	GLuint indices[] = {
		0, 1, 2,
		0, 2, 3,
	};

	//Generate VAO
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	//Create static EBO
	GLuint EBO;
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	//Create dynamic VBO
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 16, NULL, GL_DYNAMIC_DRAW);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	//Clear binds
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

//Destructor
Widget::~Widget()
{
	//Not incharge of textures
}

//	DrawTexture( texture )
//
//	Author: Michael Jordan
//	Access: protected
//	Description:
//		Draws a texture to the screen.
//		
//	Param:
//		- Texture&	|	Texture to draw.	
//
//	Return: n/a		|
//
void Widget::DrawTexture(Program* _program, Texture& texture, glm::vec2 scale)
{
	_program->Use();

	_program->SetMatrix4("ProjectionMat", Renderer::SCREEN_PROJ);

	_program->BindVertextArray(m_VAO);

	GLfloat width = texture.GetSize().x * scale.x;
	GLfloat height = texture.GetSize().y * scale.y;

	GLfloat xPos = (m_alignCanvas.x + m_position.x) - (m_alignLocal.x * width);
	GLfloat yPos = (m_alignCanvas.y + m_position.y) - ((1.0f - m_alignLocal.y) * height);

	//Create data for VBO
	GLfloat vertices[4][4] = {
		{xPos, yPos + height, 0.0f, 0.0f},
		{xPos, yPos, 0.0f, 1.0f},
		{xPos + width, yPos, 1.0f, 1.0f},
		{xPos + width, yPos + height, 1.0f, 0.0f},
	};

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

	texture.ApplyTexture("imageTexture", GL_TEXTURE0, _program);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	_program->Clear();
}