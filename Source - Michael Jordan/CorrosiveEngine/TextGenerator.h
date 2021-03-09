#ifndef _TEXT_GENERATOR_H_
#define _TEXT_GENERATOR_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	TextGenerator.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	TextGenerator
// Description:
//		Text Generator which loads .ttf information into characters.
//		Then can generate strings onto the screen.
//

//Dependency Includes
#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
#include <ft2build.h>
#include FT_FREETYPE_H

//Library Includes
#include <string>
#include <iostream>

//Predefined colours
#define WhiteText glm::vec3(1.0f, 1.0f, 1.0f)
#define BlackText glm::vec3(0.0f, 0.0f, 0.0f)
#define RedText glm::vec3(1.0f, 0.0f, 0.0f)
#define GreenText glm::vec3(0.0f, 1.0f, 0.0f)
#define BlueText glm::vec3(0.0f, 0.0f, 1.0f)

#include "Program.h"

class TextGenerator
{
	//Member Functions:
public:
	TextGenerator(std::string Font, unsigned int PixelSize);
	~TextGenerator();

	void SetColour(glm::vec3 color) { m_color = color; };

	void RenderText(std::string text, glm::vec2 position);

	glm::vec2 GetDimentionsOfText(std::string _text);
protected:

private:
	GLuint GenerateTexture(FT_Face* face);

	//Member Data:
public:

protected:
	Program* m_renderProgram = 0;
	GLuint m_VAO = 0;
	GLuint m_VBO = 0;

	std::string m_fontName;
	unsigned int m_fontSize;

	glm::vec3 m_color;

	static const int FontCharacterLimit = 128;

	struct Character
	{
		GLuint TextureID = 0;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		GLuint Advance = 0;
	} m_characters[FontCharacterLimit];

private:
	bool m_isFunctioning;
};

#endif // !_TEXT_GENERATOR_H_