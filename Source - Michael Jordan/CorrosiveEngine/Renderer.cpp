//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Renderer.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com), Callan Moore
// Class(es)	|	Renderer
// Description:
//		A class incharage of interacting with OpenGl to create programs, VAOs and textures.
//

//Parent Include
#include "Renderer.h"

//Local Includes
#include "Camera.h"
#include "Texture.h"
#include "Mesh.h"
#include "TextGenerator.h"
#include "InputHandler.h"
#include "LogManager.h"
#include "Model.h"
#include "CubeMap.h"
#include "Program.h"
#include "Shader.h"

//Library Includes
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <iostream>
#include <fstream>
#include <vector>

//Static Variables
Renderer* Renderer::sm_pInstance = 0;
int Renderer::SCREEN_WIDTH = 800;
int Renderer::SCREEN_HEIGHT = 800;
glm::mat4 Renderer::SCREEN_PROJ = glm::ortho(0.0f, (float)Renderer::SCREEN_WIDTH, 0.0f, (float)Renderer::SCREEN_HEIGHT);

//Default Constructor
Renderer::Renderer()
{
	//Pre render for draw point
	CreateSphereVAO(20, 20);
}

//Destructor
Renderer::~Renderer()
{
	//Clear vectors
	m_shaders.clear();
	m_programs.clear();

	//Delete Programs
	auto progIter = m_programs.begin();
	while (progIter != m_programs.end())
	{
		delete (*progIter).second;
		progIter = m_programs.erase(progIter);
	}

	//Delete Shaders
	auto shadIter = m_shaders.begin();
	while (shadIter != m_shaders.end())
	{
		delete (*shadIter).second;
		shadIter = m_shaders.erase(shadIter);
	}

	//Delete Textures
	auto textIter = m_pTextures.begin();
	while (textIter != m_pTextures.end())
	{
		delete (*textIter).second;
		textIter = m_pTextures.erase(textIter);
	}

	//Delete Text Generators
	auto genIter = m_pTextGenerators.begin();
	while (genIter != m_pTextGenerators.end())
	{
		delete (*genIter).second;
		genIter = m_pTextGenerators.erase(genIter);
	}

	//Delete models created
	auto modIter = m_pModels.begin();
	while (modIter != m_pModels.end())
	{
		delete (*modIter).second;
		modIter = m_pModels.erase(modIter);
	}

	//Delete Meshs
	auto meshIter = m_pMeshLibrary.begin();
	while (meshIter != m_pMeshLibrary.end())
	{
		delete (*meshIter).second;
		meshIter = m_pMeshLibrary.erase(meshIter);
	}
}

//	GetInstance( )
//
//	Author: Michael Jordan
//	Access: public 
//	Description:
//		Get function for the only instance of this class. If the instance doesn't
//		exist then it will make one.
//		
//	Param:
//		- n/a
//
//	Return: Renderer&	|	Single instance of the Renderer
//
Renderer& Renderer::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new Renderer();
	}
	return *sm_pInstance;
}

//	DestroyInstance( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Destroys the single instance of this class
//		
//	Param:
//		- n/a
//
//	Return: n/a		|
//
void Renderer::DestroyInstance()
{
	if (sm_pInstance != 0)
	{
		delete sm_pInstance;
		sm_pInstance = 0;
	}
}

//	CreateProgram( vertexShaderFilename , fragmentShaderFilename )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates, Attaches shaders to a program for graphical use. 
//
//	Param:
//		- const char*	|	[input] vertex shader name (not location).
//		- const char* 	|	[input] fragment shader name (not location).
//
//	Return: GLuint		|	Program ID
//
Program& Renderer::CreateProgram(const char* vertexShaderFilename, const char* fragmentShaderFilename)
{
	GLuint programID = 0;

	std::string programName;
	programName += vertexShaderFilename;
	programName += " | ";
	programName += fragmentShaderFilename;

	Program* program = GetProgram(programName.c_str());
	if (program != nullptr)
	{
		return *program; //Program with this name already exists.
	}

	//Creates shaders
	Shader* vShader = &CreateShader(GL_VERTEX_SHADER, vertexShaderFilename);
	Shader* fShader = &CreateShader(GL_FRAGMENT_SHADER, fragmentShaderFilename);

	// Create the program handle, attach the shaders and link it
	program = new Program(vShader, fShader);
	m_programs.insert(std::pair<std::string, Program*>(programName, program));
	return *program;
}

//	CreateShader( shaderType , shaderName )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates and compiles shader information. Will return shader if it already exists. 
//
//	Param:
//		- GLenum		|	[input] Shader Type.
//		- const char* 	|	[input] Shader location.
//
//	Return: GLuint		|	Shader ID
//
Shader& Renderer::CreateShader(GLenum shaderType, const char* shaderName)
{
	Shader* shader = GetShader(shaderName);

	if (shader != nullptr) //Shader exists?
		return *shader;

	//Create shader
	shader = new Shader(shaderType, shaderName);

	//Add to map
	m_shaders.insert(std::pair<std::string, Shader*>(shaderName, shader));

	return *shader;
}

//	CreateTexture( TextureFilename )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Generates, binds and loads an image as a texture then returns a reference.
//
//	Param:
//		- const char*	|	[input] texture name (not location).
//
//	Return: Texture&	|	Sprite struct containing texture id, width and height information.
//
Texture& Renderer::CreateTexture(const char* TextureFilename)
{
	Texture* texture = GetTexture(TextureFilename);
	GLuint textureID;

	if (texture != 0)
	{
		// Texture exists
		return *texture;
	}

	int ImageWidth, ImageHeight, ImageComponents;

	//Create file location based on local directory
	std::string vFile("Resources/Textures/");
	vFile += TextureFilename;

	//Load image data
	unsigned char* ImageData = stbi_load(vFile.c_str(), &ImageWidth, &ImageHeight, &ImageComponents, 0);

	if (ImageData) //Image exist?
	{
		//Generate and Bind Texture
		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		//Texture Parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		//Toggle type
		GLint LoadComponents = (ImageComponents == 4) ? GL_RGBA : GL_RGB;

		//Load Texture into openGl and generate Mipmap
		glTexImage2D(GL_TEXTURE_2D, 0, LoadComponents, ImageWidth, ImageHeight, 0, LoadComponents, GL_UNSIGNED_BYTE, ImageData);
		glGenerateMipmap(GL_TEXTURE_2D);

		//Add texture to map.
		texture = new Texture(ImageWidth, ImageHeight, textureID);
		m_pTextures.insert(std::pair<std::string, Texture*>(TextureFilename, texture));

		//Delete any memory unused
		stbi_image_free(ImageData);
		glBindTexture(GL_TEXTURE_2D, 0);
		return *texture;
	}
	else
	{
		texture = 0;
		std::string msg = "Failed to load texture: ";
		msg += TextureFilename;
		LogManager::GetInstance().LogErr(msg.c_str());
	}

	//Delete any memory unused
	stbi_image_free(ImageData);

	return *texture;
}

Texture& Renderer::CreateCubeMapTexture(const char* _title, std::string _textureNames[6])
{
	GLuint textureID;
	Texture* texture = GetTexture(_title);

	if (texture != 0)
	{
		// Texture exists
		return *texture;
	}

	//Load textures:
	int width, height, imageComp;
	unsigned char* imageData;

	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	for (unsigned int i = 0; i < 6; i++)
	{
		//Create full path to texture
		std::string fullPath = "Resources/Textures/CubeMap/";
		fullPath.append(_textureNames[i]);

		//Load texture into image
		imageData = stbi_load(fullPath.c_str(), &width, &height, &imageComp, 0);

		if (!imageData) //Safety check
		{
			texture = 0;
			std::string msg = "Failed to load texture: ";
			msg += _textureNames[i];
			LogManager::GetInstance().LogErr(msg.c_str());
			stbi_image_free(imageData);
			return *texture;
		}

		//Create texture in cube location
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
			0,
			GL_RGB,
			width,
			height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			imageData
		);

		//Clean up
		stbi_image_free(imageData);
	}

	//Update texture parameters
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	//Generate MipMap
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	texture = new Texture(width, height, textureID);
	texture->SetTarget(GL_TEXTURE_CUBE_MAP);
	m_pTextures.insert(std::pair<std::string, Texture*>(_title, texture));

	//Clean up
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return *texture;
}

//	CreateModel( TextureFilename )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates the model and adds it to the library for future use.
//
//	Param:
//		- const char*	|	[input] model name (not location).
//
//	Return: Model&	|	Mesh reference in the renderer library.
//
Model& Renderer::CreateModel(const char* ModelFilename)
{
	Model* model = GetModel(ModelFilename);

	if (model != 0)
	{
		// Texture exists
		return *model;
	}

	m_pModels.insert(std::pair<std::string, Model*>(ModelFilename, new Model(ModelFilename)));
	
	return *GetModel(ModelFilename);
}

//	CreateTextGenerator( font, fontSize )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates a text generator for use and saves it into a map. If
//		text generator has already been created, it will return a reference to the old one.
//		DO NOT DELETE GENERATORS RETURNED BY THIS CLASS, RENDERER WILL DECONSTRUCT IT.
//
//	Param:
//		- std::string	|	Name of font to create.
//		- unsigned int	|	size of the characters.
//
//	Return: TextGenerator*	|	Text Generator to use.
//
TextGenerator& Renderer::CreateTextGenerator(std::string font, unsigned int fontSize)
{
	TextGenerator* gen = GetTextGenerator(font, fontSize);

	std::string name = font + " | " + std::to_string(fontSize);

	if (gen != 0) // Generator exists?
	{
		return *gen;
	}

	gen = new TextGenerator(font, fontSize);

	m_pTextGenerators.insert(std::pair<std::string, TextGenerator*>(name, gen));

	return *gen;
}

//	ReadShaderFile( shaderName )
//
//	Author: Callan Moore (Provided with 3D Graphics Programming)
//	Access: private
//	Description:
//		Loads, compresses and prepares a shader file for openGl.
//
//	Param:
//		- const char* 	|	[input] Shader location.
//
//	Return: std::string	|	string of shader source code.
//
std::string Renderer::ReadShaderFile(const char *filename)
{
	// Open the file for reading
	std::ifstream file(filename, std::ios::in);
	std::string shaderCode;

	// Ensure the file is open and readable
	if (!file.good()) {
		std::cout << "Cannot read file:  " << filename << std::endl;
		return "";
	}

	// Determine the size of of the file in characters and resize the string variable to accomodate
	file.seekg(0, std::ios::end);
	shaderCode.resize((unsigned int)file.tellg());

	// Set the position of the next character to be read back to the beginning
	file.seekg(0, std::ios::beg);
	// Extract the contents of the file and store in the string variable
	file.read(&shaderCode[0], shaderCode.size());
	file.close();
	
	return shaderCode;
}

//	PrintErrorDetails( isShader, id, name )
//
//	Author: Callan Moore (Provided with 3D Graphics Programming)
//	Access: private
//	Description:
//		Logs any errors within loading the shader information.
//
//	Param:
//		- boolean 		|	[input] Boolean statement if the error is a shader.
//		- GLuint 		|	[input] Id of the buffer.
//		- const char* 	|	[input] Name of the buffer.
//
//	Return: n/a
//
void Renderer::PrintErrorDetails(bool isShader, GLuint id, const char* name)
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	(isShader == true) ? glGetShaderiv(id, GL_INFO_LOG_LENGTH, &infoLogLength) : glGetProgramiv(id, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> log(infoLogLength);

	// Retrieve the log info and populate log variable
	(isShader == true) ? glGetShaderInfoLog(id, infoLogLength, NULL, &log[0]) : glGetProgramInfoLog(id, infoLogLength, NULL, &log[0]);
	std::cout << "Error compiling " << ((isShader == true) ? "shader" : "program") << ": " << name << std::endl;
	std::cout << &log[0] << std::endl;
}

GLuint Renderer::CreateSphereVAO(const unsigned int stacks, const unsigned int sectors)
{
	GLfloat* vertices = new GLfloat[stacks * sectors * 8];
	GLuint* indices = new GLuint[stacks * sectors * 6];

	//Set vertices
	float xy, z;
	float radius = 1.0f;
	float length = 1.0f / radius; // 1.0f / radius

	float PI = 3.14159f;
	float stackStep = PI / stacks;
	float sectorStep = 2 * PI / sectors;
	float stackAngle, sectorAngle;

	unsigned int index = 0;
	for (unsigned int i = 0; i < stacks; i++)
	{
		stackAngle = PI / 2 - i * stackStep;
		xy = radius * cosf(stackAngle);
		z = radius * sinf(stackAngle);

		for (unsigned int j = 0; j < sectors; j++)
		{
			sectorAngle = j * sectorStep;

			//vertex
			vertices[index++] = xy * cosf(sectorAngle); //x
			vertices[index++] = xy * sinf(sectorAngle);	//y
			vertices[index++] = z;						//z

			//Normals
			vertices[index++] = xy * cosf(sectorAngle) * length;
			vertices[index++] = xy * sinf(sectorAngle) * length;
			vertices[index++] = z * length;

			//Tex
			vertices[index++] = (float)j / sectors;
			vertices[index++] = (float)i / stacks;
		}
	}

	//Set indices
	index = 0;
	int k1, k2;
	for (unsigned int i = 0; i < stacks; i++)
	{
		k1 = i * sectors + 1;	//Begining of this stack
		k2 = k1 + sectors + 1;	//Begining of next stack
		for (unsigned int j = 0; j < sectors; j++)
		{
			if (i != 0)
			{
				indices[index++] = k1;
				indices[index++] = k2;
				indices[index++] = k1 + 1;
			}

			if (i != sectors - 1)
			{
				//k1+1, k2, k2+1
				indices[index++] = k1 + 1;
				indices[index++] = k2;
				indices[index++] = k2 + 1;
			}

			k1++;
			k2++;
		}
	}
	GLsizeiptr verticeSize = sizeof(GLfloat) * stacks * sectors * 8;
	GLsizeiptr indicesSize = sizeof(GLuint) * stacks * sectors * 6;

	GLuint VAO = CreateVAO(BUFFER_POS | BUFFER_NORM | BUFFER_TEX, verticeSize, indicesSize, vertices, indices);

	delete [] vertices;
	delete [] indices;

	return VAO;
}

void Renderer::CheckGLErrors()
{
	GLenum err;

	while ((err = glGetError()) != GL_NO_ERROR)
	{
		//Log Error
		std::string errorString = "OpenGl Error: ";
		errorString += reinterpret_cast<const char*>(gluErrorString(err));

		LogManager::GetInstance().LogErr(errorString.c_str());
	}
}

Program* Renderer::GetProgram(const char* programName)
{
	std::map<std::string, Program*>::iterator it;
	it = m_programs.find(programName);
	if (it != m_programs.end())
	{
		return (*it).second;
	}
	return nullptr;
}

//	GetShader( shaderName )
//
//	Author: Michael Jordan
//	Access: private
//	Description:
//		Returns the shader ID associated to the shader name. Returns 0 if the shader doesn't exist.
//
//	Param:
//		- const char* 	|	[input] Shader name (not location).
//
//	Return: GLuint		|	Shader ID
//
Shader* Renderer::GetShader(const char* shaderName)
{
	std::map<std::string, Shader*>::iterator it;
	it = m_shaders.find(shaderName);
	if (it != m_shaders.end())
	{
		return (*it).second;
	}
	return nullptr;
}

//	CreateVAO( type,  vertCount, indCount, vertData, indData )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates a VAO of a specific type.
//		Define which type using bitwise operators and {BUFFER_POS, BUFFER_COL, BUFFER_TEX}.
//
//	Param:
//		- uint32_t 		|	[input] Bit type of the VAO.
//		- GLsizeiptr 	|	[input] size of the vertices data.
//		- GLsizeiptr 	|	[input] size of the indices data.
//		- GLfloat* 		|	[input] pointer to the first vertices data.
//		- GLuint* 		|	[input] pointer to the first indices data.
//
//	Return: GLuint		|	VAO ID
//
GLuint Renderer::CreateVAO(uint32_t type, GLsizeiptr vertCount, GLsizeiptr indCount,  GLfloat* vertData, GLuint* indData)
{
	GLuint bufferID = 0;

	//Safety Check
	assert(vertData != 0);
	assert(indData != 0);

	unsigned int vectSize = GetVectorSize(type);
	unsigned int vectOffset = 0;
	GLuint attributeID = 0;

	//Generate & bind VAO buffer
	glGenVertexArrays(1, &bufferID);
	if (bufferID == 0)
	{
		CheckGLErrors();
		assert(false);
	}

	glBindVertexArray(bufferID);

	//Generate & bind EBO
	GLuint EBO_ID = CreateEBO(indData, indCount);
	
	//Generate & bind VBO
	GLuint VBO_ID = CreateVBO(vertData, vertCount);
	
	//Position
	if (type & BUFFER_POS)
	{
		glVertexAttribPointer(attributeID, 3, GL_FLOAT, GL_FALSE, vectSize * sizeof(GLfloat), (GLvoid*)(vectOffset * sizeof(GLfloat)));
		glEnableVertexAttribArray(attributeID);
		vectOffset += 3;
		attributeID++;
	}

	//Color
	if (type & BUFFER_NORM)
	{
		glVertexAttribPointer(attributeID, 3, GL_FLOAT, GL_FALSE, vectSize * sizeof(GLfloat), (GLvoid*)(vectOffset * sizeof(GLfloat)));
		glEnableVertexAttribArray(attributeID);
		vectOffset += 3;
		attributeID++;
	}
	
	//Texture
	if (type & BUFFER_TEX)
	{
		glVertexAttribPointer(attributeID, 2, GL_FLOAT, GL_FALSE, vectSize * sizeof(GLfloat), (GLvoid*)(vectOffset * sizeof(GLfloat)));
		glEnableVertexAttribArray(attributeID);
		vectOffset += 2;
		attributeID++;
	}
	
	return bufferID;
}

//	CreateDefaultProgram( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates, Attaches shaders to a default program for graphical use. 
//
//	Param:
//		- n/a
//
//	Return: Program		|	Program
//
Program& Renderer::CreateDefaultProgram()
{
	return CreateProgram("DefaultVert.vs", "DefaultFrag.fs");
}

//	CreateDefaultVAO( vertCount, indCount, vertData, indData )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates a Default VAO with Position, colour and texture UV data.
//
//	Param:
//		- GLsizeiptr 	|	[input] size of the vertices data.
//		- GLsizeiptr 	|	[input] size of the indices data.
//		- GLfloat* 		|	[input] pointer to the first vertices data.
//		- GLuint* 		|	[input] pointer to the first indices data.
//
//	Return: GLuint		|	VAO ID
//
GLuint Renderer::CreateDefaultVAO(GLsizeiptr vertCount, GLsizeiptr indCount, GLfloat* vertData, GLuint* indData)
{
	GLuint bufferID;

	//Safety Check
	assert(vertData != 0);
	assert(indData != 0);

	//Generate & bind VAO buffer
	glGenVertexArrays(1, &bufferID);
	glBindVertexArray(bufferID);

	//Generate & bind EBO
	GLuint EBO_ID = CreateEBO(indData, indCount);

	//Generate & bind VBO
	GLuint VBO_ID = CreateVBO(vertData, vertCount);

	//Position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(0 * sizeof(GLfloat)));
	glEnableVertexAttribArray(0);

	//Normals
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//UV
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	return bufferID;
}

//	CreateVBO( data , size )
//
//	Author: Michael Jordan
//	Access: private
//	Description:
//		Generates, Binds and buffers vertices data into an VBO.
//
//	Param:
//		- GLfloat*		|	[input] unsigned float array of each vertices.
//		- GLsizeiptr	|	[input] size of the vertices array.
//
//	Return: GLuint		|	VBO buffer ID
//
GLuint Renderer::CreateVBO(GLfloat* data, GLsizeiptr size)
{
	GLuint bufferID = 0;

	//Generate buffer
	glGenBuffers(1, &bufferID);
	if (bufferID == 0)
	{
		CheckGLErrors();
		assert(false);
	}

	//Bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, bufferID);

	//Assign data to buffer
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	return bufferID;
}

//	CreateEBO( data , size )
//
//	Author: Michael Jordan
//	Access: private
//	Description:
//		Generates, Binds and buffers indice data into an EBO.
//
//	Param:
//		- GLuint*		|	[input] unsigned integer array of each indice.
//		- GLsizeiptr	|	[input] size of the indices array.
//
//	Return: GLuint		|	EBO buffer ID
//
GLuint Renderer::CreateEBO(GLuint* data, GLsizeiptr size)
{
	GLuint bufferID = 0;

	//Generate buffer
	glGenBuffers(1, &bufferID);
	if (bufferID == 0)
	{
		CheckGLErrors();
		assert(false);
	}

	//Bind buffer
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bufferID);

	//Assign data to buffer
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	return bufferID;
}

//	GetVectorSize( type )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Converts the bitwise operator to determin the vector size.
//
//	Param:
//		- uint32_t		|	[input] Bit variable.
//
//	Return: unsigned int|	The size of the vector
//
unsigned int Renderer::GetVectorSize(uint32_t type)
{
	unsigned int size = 0;

	if (type & BUFFER_POS)
	{
		size += 3;	//x, y, z,
	}

	if (type & BUFFER_NORM)
	{
		size += 3; //r, g, b
	}

	if (type & BUFFER_TEX)
	{
		size += 2; //u, v,
	}

	return size;
}

glm::vec3 Renderer::GetMouseDirectionToWorld(Camera* camera)
{
	//https://antongerdelan.net/opengl/raycasting.html

	//Get Mouse position
	glm::vec2 mousePos = InputHandler::GetInstance().GetMousePos();

	//Get Normalised Homogeneous Coordinates
	glm::vec4 normalised;
	normalised.x = (2.0f * mousePos.x) / SCREEN_WIDTH - 1;
	normalised.y = 1.0f - (2.0f * mousePos.y) / SCREEN_HEIGHT;
	normalised.z = -1.0f;
	normalised.w = 1.0f;

	//Eye coordinates
	glm::vec4 eye = glm::inverse(camera->GetProjMatrix()) * normalised;
	eye.z = -1.0f;
	eye.w = 0.0f;

	//World Coordinates
	glm::vec3 world = glm::vec3(glm::inverse(camera->GetViewMatrix()) * eye);

	//Return a point on the plane.
	return glm::normalize(world);
}

glm::vec3 Renderer::GetPositonFromRayOnPlane(glm::vec3 origin, glm::vec3 direction, glm::vec3 norm, float dist)
{
	float numer = glm::dot(origin,  glm::normalize(norm)) + dist;
	float denom = glm::dot(glm::normalize(direction), glm::normalize(norm));

	float t = -numer / denom;
	
	return origin + direction * t;
}

//	DrawPoint( camera, point, colour )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Draw a sphere to be a point in world space.
//
//	Param:
//		- Camera*		|	Camera to project point from
//		- glm::vec3		|	Point to draw at.
//		- glm::vec3		|	Color to draw.
//
//	Return: n/a		|
//
void Renderer::DrawPoint(Camera* camera,  glm::vec3 point, glm::vec3 colour)
{
	//Get Default shader program
	Program* program = &CreateDefaultProgram();

	//Get Sphere with 20 stacks and 20 sectors
	Mesh* temp = &CreateSphereMesh(20, 20);

	//Create a transform at the desired world location
	Transform transform;
	transform.GetPositionRef() = point;
	transform.GetScaleRef() = glm::vec3(0.25f, 0.25f, 0.25f);

	//Adjust the mesh's texture and color
	Texture* texture = &CreateTexture("DefaultTexture.png");
	texture->ApplyTexture("imageTexture", GL_TEXTURE0, program);
	temp->SetColour(glm::vec4(colour, 1.0f));

	//Draw the sphere to the world.
	temp->Draw(camera, program, transform);

	//Do not delete temp, it is saved for next use.
}

//Scrapped Code (To Come back to)
void Renderer::DrawLine(Camera* camera, glm::vec3 origin, glm::vec3 direction, float distance, glm::vec3 colour, float thickness)
{
	glm::vec3 pointA = origin;
	glm::vec3 pointB = origin + glm::normalize(direction) * distance;
	
	DrawLineBetween(camera, pointA, pointB, colour, thickness);
}

//Scrapped Code (To Come back to)
void Renderer::DrawLineBetween(Camera* camera, glm::vec3 pointA, glm::vec3 pointB, glm::vec3 colour, float thickness)
{
	//glBegin(GL_LINES);
	//	glLineWidth(thickness);
	//	glPushMatrix();
	//		glLoadMatrixf(glm::value_ptr(glm::inverse(camera->GetProj())));
	//		glColor3f(colour.x, colour.y, colour.z);
	//		glVertex3f(pointA.x, pointA.y, pointA.z);
	//		glVertex3f(pointB.x, pointB.z, pointB.z);
	//		glColor3f(1.0f, 1.0f, 1.0f);
	//	glPopMatrix();
	//	glLineWidth(1.0f);
	//glEnd();
}

//	CreateSquareMesh( _mesh )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates and returns the address to the square mesh.
//
//	Param:
//		- Mesh&		|	mesh address to set VAO to a square
//
//	Return: const Mesh&		|	Reference to the square mesh stored within the Renderer.
//
Mesh& Renderer::CreateSquareMesh()
{
	std::map<std::string, Mesh*>::iterator iter = m_pMeshLibrary.find("Square");
	if (iter == m_pMeshLibrary.end()) //Square doesn't exist
	{
		LogManager::GetInstance().Log("Square Mesh Created.");
		//Create temperary vertices
		GLfloat vertices[] =
		{
			//Positon				//Normals			//UV
			 0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 0.0f,		//Top Right
			-0.5f, 0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 0.0f,		//Top left
			-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	0.0f, 1.0f,		//Bottem Left
			 0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,	1.0f, 1.0f,		//Bottem Right
		};
	
		//Create temperary indices
		GLuint indices[] =
		{
			0, 1, 2,
			2, 3, 0,
		};
	
		GLuint VAO = CreateDefaultVAO(sizeof(vertices), sizeof(indices), vertices, indices);
		unsigned int indicesCount = sizeof(indices) / sizeof(GLuint);
		iter = m_pMeshLibrary.insert(std::pair<std::string, Mesh*>("Square", new Mesh(VAO, indicesCount))).first;
	}

	return *(iter->second);
}

//	CreateCubeMesh( _mesh )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Updates a mesh provdied to have the same VAO as a cube (same size as the screen).
//
//	Param:
//		- Mesh&		|	mesh address to set VAO to a cube
//
//	Return: n/a		|
//
Mesh& Renderer::CreateCubeMesh()
{
	std::map<std::string, Mesh*>::iterator iter = m_pMeshLibrary.find("Cube");
	if (iter == m_pMeshLibrary.end()) //Cube doesn't exist
	{
		LogManager::GetInstance().Log("Cube Mesh Created.");

		float temp = 1.0f / 3.0f;
		GLfloat vertices[] =
		{
			//Refer to Cube Texture

			//Positon					//Normals				//UV
				1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		temp, 0.0f,	//0 [1] Top-Right
				-1.0f, 1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f,	//1 [1] Top-Left
				-1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.5f, //2 [1] Bottem-Left
				1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f,		temp, 0.5f, //3 [1] Bottem-Right

				1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f,		2*temp, 0.0f,	//4 [2] Top-Right
				-1.0f, -1.0f, 1.0f,		0.0f, -1.0f, 0.0f,		temp, 0.0f,		//5 [2] Top-Left
				-1.0f, -1.0f, -1.0f,	0.0f, -1.0f, 0.0f,		temp, 0.5f,		//6 [2] Bottem-Left
				1.0f, -1.0f, -1.0f,		0.0f, -1.0f, 0.0f,		2 * temp, 0.5f,	//7 [2] Bottem-Right

				1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f,		3 * temp, 0.0f,	//8 [3] Top-Right
				1.0f, -1.0f, 1.0f,		1.0f, 0.0f, 0.0f,		2 * temp, 0.0f,	//9 [3] Top-Left
				1.0f, -1.0f, -1.0f,		1.0f, 0.0f, 0.0f,		2 * temp, 0.5f,	//10 [3] Bottem-Left
				1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f,		3 * temp, 0.5f,	//11 [3] Bottem-Right

				-1.0f, -1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,		temp, 0.5f,	//12 [4] Top-Right
				-1.0f, 1.0f, 1.0f,		-1.0f, 0.0f, 0.0f,		0.0f, 0.5f,	//13 [4] Top-Left
				-1.0f, 1.0f,-1.0f,		-1.0f, 0.0f, 0.0f,		0.0f, 1.0f, //14 [4] Bottem-Left
				-1.0f, -1.0f, -1.0f,	-1.0f, 0.0f, 0.0f,		temp, 1.0f, //15 [4] Bottem-Right

				-1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,		2 * temp, 0.5f,	//16 [5] Top-Right
				1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f,		temp, 0.5f,		//17 [5] Top-Left
				1.0f, 1.0f,-1.0f,		0.0f, 1.0f, 0.0f,		temp, 1.0f,		//18 [5] Bottem-Left
				-1.0f, 1.0f, -1.0f,		0.0f, 1.0f, 0.0f,		2 * temp, 1.0f,	//19 [5] Bottem-Right

				1.0f, -1.0f, -1.0f,		0.0f, 0.0f, -1.0f,		3 * temp, 0.5f,	//20 [6] Top-Right
				-1.0f, -1.0f, -1.0f,	0.0f, 0.0f, -1.0f,		2 * temp, 0.5f,	//21 [6] Top-Left
				-1.0f, 1.0f, -1.0f,		0.0f, 0.0f, -1.0f,		2 * temp, 1.0f,	//22 [6] Bottem-Left
				1.0f, 1.0f, -1.0f,		0.0f, 0.0f, -1.0f,		3 * temp, 1.0f,	//23 [6] Bottem-Right
		};

		GLuint indices[36];
		for (int i = 0; i < 6; i++)
		{
			indices[i * 6 + 0] = 0 + 4 * i;
			indices[i * 6 + 1] = 1 + 4 * i;
			indices[i * 6 + 2] = 2 + 4 * i;

			indices[i * 6 + 3] = 2 + 4 * i;
			indices[i * 6 + 4] = 3 + 4 * i;
			indices[i * 6 + 5] = 0 + 4 * i;
		}

		GLuint VAO = CreateDefaultVAO(sizeof(vertices), sizeof(indices), vertices, indices);
		unsigned int indicesCount = sizeof(indices) / sizeof(GLuint);

		iter = m_pMeshLibrary.insert(std::pair<std::string, Mesh*>("Cube", new Mesh(VAO, indicesCount))).first;
	}
	return *(iter->second);
}

//	CreateCubeMesh( _mesh )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Updates a mesh provdied to have the same VAO as a sphere (same size as the screen).
//
//	Param:
//		- Mesh&		|	mesh address to set VAO to a sphere
//
//	Return: n/a		|
//
Mesh& Renderer::CreateSphereMesh(unsigned int stacks, unsigned int sectors)
{
	std::string name = "Sphere_" + std::to_string(stacks) + "x" + std::to_string(sectors);

	std::map<std::string, Mesh*>::iterator iter = m_pMeshLibrary.find(name);
	if (iter == m_pMeshLibrary.end()) //Sphere doesn't exist
	{
		std::string temp = name + " Mesh Created.";
		LogManager::GetInstance().Log(temp.c_str());

		GLuint VAO = CreateSphereVAO(stacks, sectors); //Creates VAO and adds to library.

		unsigned int indicesCount = stacks * sectors * 6;

		iter = m_pMeshLibrary.insert(std::pair<std::string, Mesh*>(name, new Mesh(VAO, indicesCount))).first;
	}

	return *(iter->second);
}

Mesh& Renderer::CreateCubeMapMesh()
{
	std::map<std::string, Mesh*>::iterator iter = m_pMeshLibrary.find("CubeMap");
	if (iter == m_pMeshLibrary.end())
	{
		LogManager::GetInstance().Log("CubeMap Mesh Created.");
		GLfloat vertices[] =
		{
			//Refer to Cube Texture

			//Positon				
				1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, 1.0f,

				1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, 1.0f,
				-1.0f, -1.0f, -1.0f,
				1.0f, -1.0f, -1.0f,

				1.0f, 1.0f, 1.0f,
				1.0f, -1.0f, 1.0f,
				1.0f, -1.0f, -1.0f,
				1.0f, 1.0f, -1.0f,

				-1.0f, -1.0f, 1.0f,
				-1.0f, 1.0f, 1.0f,
				-1.0f, 1.0f,-1.0f,
				-1.0f, -1.0f, -1.0f,

				-1.0f, 1.0f, 1.0f,
				1.0f, 1.0f, 1.0f,
				1.0f, 1.0f,-1.0f,
				-1.0f, 1.0f, -1.0f,

				1.0f, -1.0f, -1.0f,
				-1.0f, -1.0f, -1.0f,
				-1.0f, 1.0f, -1.0f,
				1.0f, 1.0f, -1.0f,
		};

		GLuint indices[36];
		for (int i = 0; i < 6; i++)
		{
			indices[i * 6 + 0] = 0 + 4 * i;
			indices[i * 6 + 1] = 2 + 4 * i;
			indices[i * 6 + 2] = 1 + 4 * i;

			indices[i * 6 + 3] = 2 + 4 * i;
			indices[i * 6 + 4] = 0 + 4 * i;
			indices[i * 6 + 5] = 3 + 4 * i;
		}

		
		GLuint VAO = CreateVAO(BUFFER_POS, 24 * 3 * sizeof(GLuint), 36 * sizeof(GLfloat), vertices, indices);
		unsigned int indicesCount = sizeof(indices) / sizeof(GLuint);

		iter = m_pMeshLibrary.insert(std::pair<std::string, Mesh*>("CubeMap", new Mesh(VAO, indicesCount))).first;

		return *(iter->second);
	}
	return *(iter->second);
}

//	GetTexture( TextureFilename )
//
//	Author: Michael Jordan
//	Access: private
//	Description:
//		Gets the sprite if the sprite exists, else returning 0.
//
//	Param:
//		- const char*	|	[input] texture name (not location).
//
//	Return: Texture*	|	pointer to Sprite structure.
//
Texture* Renderer::GetTexture(const char* TextureFilename)
{
	std::map<std::string, Texture*>::iterator it;
	it = m_pTextures.find(TextureFilename);
	if (it != m_pTextures.end())
	{
		return (*it).second;
	}
	return nullptr;
}

//	GetModel( ModelFilename )
//
//	Author: Michael Jordan
//	Access: private
//	Description:
//		Gets the model if the model exists, else returning 0.
//
//	Param:
//		- const char*	|	[input] texture name (not location).
//
//	Return: Model*	|	pointer to Sprite structure.
//
Model* Renderer::GetModel(const char* ModelFilename)
{
	std::map<std::string, Model*>::iterator it;
	it = m_pModels.find(ModelFilename);
	if (it != m_pModels.end())
	{
		return (*it).second;
	}
	return nullptr;
}

//	GetTextGenerator( FontFilename, fontSize )
//
//	Author: Michael Jordan
//	Access: private
//	Description:
//		Checks the map if the text generator has already been created, returning
//		nullptr if the map doesn't exist.
//
//	Param:
//		- std::string	|	Name of font to create.
//		- unsigned int	|	size of the characters.
//
//	Return: TextGenerator*	|	Text Generator to use.
//
TextGenerator* Renderer::GetTextGenerator(std::string FontFilename, unsigned int fontSize)
{
	std::map<std::string, TextGenerator*>::iterator it;
	std::string name = FontFilename + " | " + std::to_string(fontSize);

	it = m_pTextGenerators.find(name);
	if (it != m_pTextGenerators.end())
	{
		return (*it).second;
	}
	return 0;
}