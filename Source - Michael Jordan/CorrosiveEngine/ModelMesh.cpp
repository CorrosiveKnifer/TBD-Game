//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	ModelMesh.h
// Author(s)    |	Callan Moore, Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	ModelMesh
// Description:
//		Mesh derived from a model loaded from the model.h
//

//Self Include
#include "ModelMesh.h"

//Constructor
ModelMesh::ModelMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	
	this->textures = textures;

	//this->m_localTransform.GetScaleRef() = glm::vec3(0.2f, 0.2f, 0.2f);
	// Now that we have all the required data, set the vertex buffers and its attribute pointers.
	this->SetupMesh();
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
void ModelMesh::Draw(Camera* camera, Program* program, Transform transform)
{
	program->Use();

	// Bind appropriate textures
	GLuint diffuseNr = 1;
	GLuint specularNr = 1;

	for (GLuint i = 0; i < this->textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i); // Active proper texture unit before binding
										  // Retrieve texture number (the N in diffuse_textureN)

		// Now set the sampler to the correct texture unit
		program->SetInteger("imageTexture", i);
		// And finally bind the texture
		glBindTexture(GL_TEXTURE_2D, this->textures[i].id);
	}

	// Draw mesh
	Mesh::Draw(camera, program, transform);

	// Always good practice to set everything back to defaults once configured.
	for (GLuint i = 0; i < this->textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

//	SetupMesh( )
//
//	Author: Michael Jordan
//	Access: private
//	Description:
//		Load the mesh into opengl.
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void ModelMesh::SetupMesh()
{
	// Create buffers/arrays
	glGenVertexArrays(1, &this->m_VAO_ID);
	glGenBuffers(1, &this->VBO);
	glGenBuffers(1, &this->EBO);

	glBindVertexArray(this->m_VAO_ID);
	// Load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
	// A great thing about structs is that their memory layout is sequential for all its items.
	// The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
	// again translates to 3/2 floats which translates to a byte array.
	glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(Vertex), &this->vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), &this->indices[0], GL_STATIC_DRAW);

	m_indicesCount = this->indices.size();
	// Set the vertex attribute pointers
	// Vertex Positions
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	// Vertex Normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
	// Vertex Texture Coords
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}