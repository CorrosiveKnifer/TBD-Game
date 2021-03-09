#ifndef _MODEL_MESH_H_
#define _MODEL_MESH_H_

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

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

// GL Includes

// EDIT
#include "Camera.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Program.h"
#include <scene.h>
// EDIT END

struct Vertex {
	
	glm::vec3 Position; // Position	
	glm::vec3 Normal; // Normal	
	glm::vec2 TexCoords; // TexCoords
};

struct MeshTexture {
	GLuint id = 0;
	string type;
	aiString path;
};

class ModelMesh : public Mesh {
public:
	/*  Mesh Data  */
	vector<Vertex> vertices;
	vector<GLuint> indices;
	vector<MeshTexture> textures;

	/*  Functions  */
	// Constructor
	ModelMesh(vector<Vertex> vertices, vector<GLuint> indices, vector<MeshTexture> textures);

	virtual ~ModelMesh() {};

	// Render the mesh
	virtual void Draw(Camera* camera, Program* program, Transform transform);

private:
	/*  Render data  */
	GLuint VBO, EBO;

	/*  Functions    */
	// Initializes all the buffer objects/arrays
	void SetupMesh();
};

#endif