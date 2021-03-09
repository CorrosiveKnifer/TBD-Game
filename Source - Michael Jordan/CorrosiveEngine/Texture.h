#ifndef _TEXTURE_H_
#define _TEXTURE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Texture.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com),
// Class(es)	|	Texture
// Description:
//		A class which handles the binding and displaying of a Texture.
//

#include "Program.h" //<--- Dependanies include

class Texture
{
	//Member Functions:
public:
	Texture(unsigned int width, unsigned int height, GLuint ID) 
		: m_width(width), m_height(height), m_textureID(ID), m_target(GL_TEXTURE_2D){ };

	Texture(const Texture& origin ) 
	{
		this->m_width = origin.m_width;
		this->m_height = origin.m_height;
		this->m_textureID = origin.m_textureID;
		this->m_target = origin.m_target;
	}

	~Texture() {};

	void ApplyTexture(std::string name, GLenum _slot, Program* _program) const 
	{
		glActiveTexture(_slot);
		glBindTexture(m_target, m_textureID);
		_program->SetInteger(name.c_str(), (_slot - GL_TEXTURE0));
	};
	
	//Set Functions
	void SetSize(glm::uvec2 size) { m_width = size.x; m_height = size.y; };
	void SetTarget(GLenum _target) { m_target = _target; };
	//Get Functions
	glm::uvec2 GetSize() { return glm::uvec2(m_width, m_height); };
	GLuint GetID() { return m_textureID; };
protected:

private:

	//Member Data:
public:

protected:

	unsigned int m_width;
	unsigned int m_height;
	GLuint m_textureID;
	GLenum m_target;
private:

};

#endif // !_TEXTURE_H_