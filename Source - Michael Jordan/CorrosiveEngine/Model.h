#ifndef _MODEL_H_
#define _MODEL_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Model.h
// Author(s)    |	Callan Moore, Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Model
// Description:
//		Main class to interact with the ASimp library to import models into
//		world space.
//

// Std. Includes
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>

using namespace std;

// GL Includes
#include <glew.h>
#include <freeglut.h>
#include <stb_image.h>

//Depencancies
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>

// Assimp 
#include <Importer.hpp>
#include <scene.h>
#include <postprocess.h>

//Local Includes
#include "Renderer.h"
#include "ModelMesh.h"
#include "Camera.h"

class Model
{
	//Member Function
public:
	Model(std::string path);
	Model(const Model& model);
	~Model() {};

	// Draws the model, and thus all its meshes
	void Draw(Camera* camera, Program* program, Transform transform) const;

	//Change material color
	void SetColour(glm::vec4 _colour) { m_colour = _colour; };

	//void SetTexture(Texture& _texture);
protected:

private:
	void LoadModel(string path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	ModelMesh* ProcessMesh(aiMesh* mesh, const aiScene* scene);
	vector<MeshTexture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
	GLint TextureFromFile(const char* path, string directory);

	//Member Data:
public:

protected:

private:
	/*  Model Data  */
	vector<Mesh*> m_components;
	string directory;
	vector<MeshTexture> textures_loaded;	// Stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
	glm::vec4 m_colour;
};

#endif