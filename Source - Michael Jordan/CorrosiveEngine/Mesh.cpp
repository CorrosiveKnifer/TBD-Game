//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Mesh.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Mesh
// Description:
//		Mesh class which generates the VAO into the world.
//

#include "Mesh.h"
#include "Texture.h"
#include "LightingSystem.h"

//Default Constructor
Mesh::Mesh()
{
	m_VAO_ID = 0;
	m_inheritedMatrix = glm::mat4();
}

Mesh::Mesh(const GLuint _VAO, unsigned int _indices)
{
	m_VAO_ID = _VAO;
	m_indicesCount = _indices;
}


//Destuctor
Mesh::~Mesh()
{
	
}

//	Draw( camera, program )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Draws the mesh into world space.
//		
//	Param:
//		- Camera*	|	Main camera to project to
//		- GLuint	|	Render program ID
//
//	Return: n/a		|
//
void Mesh::Draw(Camera* camera, Program* program, Transform transform) const
{
	//Program is in use once this is called...

	LightingSystem::GetInstance().ApplyLights(program, transform.GetPosition());
	
	//Load Matrix information
	glm::mat4 MVP = (camera != 0) ? camera->GetCamMatrix() : glm::mat4();
	glm::vec3 camPos = (camera != 0) ? camera->GetWorldPosition() : glm::vec3(1.0f, 0.0f, 0.0f);

	program->SetVector3("camPos", camPos);
	program->SetMatrix4("MVP", MVP);
	program->SetMatrix4("transform", (m_inheritedMatrix * transform.GetModelMatrix()));
	program->SetFloat("shininess", m_shininess);

	//Colour Uniforms
	program->SetVector4("imageColour", m_meshColour);

	//UV manipulation
	program->SetVector2("texOffset", m_textureOffset);
	program->SetVector2("frame", m_frameTexSize);

	//Draw Image
	program->BindVertextArray(m_VAO_ID);
	glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, 0);
}