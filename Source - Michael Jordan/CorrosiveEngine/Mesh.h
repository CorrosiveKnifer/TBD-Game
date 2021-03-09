#ifndef _MESH_H_
#define _MESH_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Mesh.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Mesh
// Description:
//		A mesh is a component which is incharge of rendering itself to the world.
//

//Parent Include
#include "Component.h"

//Library Includes
#include <vector>

//Forward Declaration
class Texture;

class Mesh : public Component
{
	//Member Functions:
public:
	Mesh();
	Mesh(const GLuint _VAO, unsigned int _indices);
	virtual ~Mesh();

	virtual void Draw(Camera* cam, Program* program, Transform transform) const;
	virtual void Process(float dt) {/*Do nothing*/};

	void SetInheritedMatrix(glm::mat4 _matrix) { m_inheritedMatrix = _matrix; };
	//glm::mat4 GetModelMatrix() { return m_localTransform.GetModelMatrix(); };
	
	void SetRelativeOrigin(glm::vec3 origin) { m_relativeOrigin = origin; };
	void SetTextureOffset(glm::vec2 textureOffset) { m_textureOffset = textureOffset; };
	
	void SetColour(glm::vec4 newColour) { m_meshColour = newColour; };

	void SetVAO(GLuint _VAO, unsigned int _indices) { m_VAO_ID = _VAO; m_indicesCount = _indices; };
	GLuint GetVAO() { return m_VAO_ID; };
	
	void SetShininess(float _shine) { m_shininess = _shine; };

protected:
	

private:

	//Member Data:
public:

protected:
	//Texture* m_texture;

	GLuint m_VAO_ID = 0;
	unsigned int m_indicesCount = 0;

	glm::vec4 m_meshColour = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	glm::vec3 m_relativeOrigin = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec2 m_frameTexSize = glm::vec2(1.0f, 1.0f);
	glm::vec2 m_textureOffset = glm::vec2(0.0f, 0.0f);

	float m_shininess = 32.0f;

	glm::mat4 m_inheritedMatrix;
private:
	
};

#endif // !_MESH_H_