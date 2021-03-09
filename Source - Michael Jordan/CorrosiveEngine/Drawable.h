#ifndef _DRAWABLE_H_
#define _DRAWABLE_H_
//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Drawable.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Drawable(Abstract)
// Description:
//		A ComputerAI class incharge of selecting appropriate moves based on difficulty.
//		An Easy difficulty, which is indicated with a type of 0, will select a random move.
//		A Hard difficulty, which is indicated with a type of 1, will use MINIMAX to select 
//		the best move.
//

//Local Includes
#include "Program.h"//<--- Dependencies Includes
#include "Camera.h"

class Drawable
{
	//Member Functions:
public:
	virtual void Draw(Camera* camera, Program* program) = 0; //To render object to the screen.

	virtual void SetLocalPosition(glm::vec3 position) { m_position = position; UpdateModelMatrix(); };
	virtual void SetLocalRotation(glm::vec3 rotation) { m_rotation = rotation; UpdateModelMatrix(); };
	virtual void SetLocalScale(glm::vec3 scale) { m_scale = scale; UpdateModelMatrix(); };

	glm::vec3 GetLocalPosition() const { return m_position; };
	glm::vec3 GetLocalRotation() const { return m_rotation; };
	glm::vec3 GetLocalScale() const { return m_scale; };

protected:
	//Default Constructor
	Drawable() 
	{
		m_position = glm::vec3(0.0f, 0.0f, 0.0f);
		m_rotation = glm::vec3(0.0f, 0.0f, 0.0f);
		m_scale = glm::vec3(1.0f, 1.0f, 1.0f);
	};

	//Destructor
	virtual ~Drawable()
	{
		
	};

	//Drawable(const Drawable& drawable);
	Drawable& operator=(const Drawable& drawable);

	void UpdateModelMatrix()
	{
		m_transform = glm::translate(glm::mat4(), m_position); //Translate position
		m_transform = glm::rotate(m_transform, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
		m_transform = glm::rotate(m_transform, glm::radians(m_rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_transform = glm::rotate(m_transform, glm::radians(m_rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_transform = glm::scale(m_transform, m_scale);
	};

private:

	//Member Data:
public:

protected:
	//Process Matrix
	glm::mat4 m_transform;

	//Local
	glm::vec3 m_position;
	glm::vec3 m_rotation;
	glm::vec3 m_scale;

private:

};


#endif // !_DRAWABLE_H_
