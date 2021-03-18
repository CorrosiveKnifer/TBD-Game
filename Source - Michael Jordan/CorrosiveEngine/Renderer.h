#ifndef _RENDERER_H_
#define _RENDERER_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Renderer.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com), Callan Moore 
// Class(es)	|	Renderer
// Description:
//		A class incharage of interacting with OpenGl to create programs, VAOs and textures.
//

// Dependency Includes
#include <glew.h>
#include <freeglut.h>
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/quaternion.hpp>
#include <gtx/quaternion.hpp>
#include <gtc/type_ptr.hpp>

//Library Includes
#include <string>
#include <map>
#include <assert.h>

//Bitwise
#define BUFFER_POS (1 << 0)
#define BUFFER_NORM (1 << 1)
#define BUFFER_TEX (1 << 2)

//Forward Declaration
class Camera;
class Texture;
class Mesh;
class TextGenerator;
class Model;
class CubeMap;
class Program;
class Shader;

struct UtilityVAO
{
	GLuint _program;
	GLuint _VAO;
	GLuint _VBO;
};

class Renderer
{
	//Member Functions:
public:
	static Renderer& GetInstance();
	static void DestroyInstance();

#pragma region Creation Functions
	//OpenGL Creation functions
	Program& CreateDefaultProgram();
	Program& CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	GLuint CreateDefaultVAO(GLsizeiptr vertCount, GLsizeiptr indCount, GLfloat* vertData, GLuint* indData);
	GLuint CreateVAO(uint32_t type, GLsizeiptr vertCount, GLsizeiptr indCount, GLfloat* vertData, GLuint* indData);

	//Texture Creation functions
	Texture& CreateTexture(const char* TextureFilename);
	Texture& CreateCubeMapTexture(const char* _title, std::string _textureNames[6]);

	//Mesh Creation functions
	Model& CreateModel(const char* ModelFilename);
	Mesh& CreateCircleMesh(unsigned int stacks);
	Mesh& CreateSquareMesh();
	Mesh& CreateCubeMesh();
	Mesh& CreateSphereMesh(unsigned int stacks, unsigned int sectors);
	Mesh& CreateCubeMapMesh();

	//Text Creation functions
	TextGenerator& CreateTextGenerator(std::string font, unsigned int fontSize);
#pragma endregion	

	static unsigned int GetVectorSize(uint32_t type);

	//World Functions
	glm::vec3 GetMouseDirectionToWorld(Camera* camera);
	glm::vec3 GetPositonFromRayOnPlane(glm::vec3 origin, glm::vec3 direction, glm::vec3 norm, float dist = 0);
	
	//Direct Draw functions
	void DrawPoint(Camera* camera, glm::vec3 point, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f));
	void DrawLine(Camera* camera, glm::vec3 origin, glm::vec3 direction, float distance, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f), float thickness = 1.0f);
	void DrawLineBetween(Camera* camera, glm::vec3 pointA, glm::vec3 pointB, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f), float thickness = 1.0f);
protected:
	
private:
	Renderer();
	~Renderer();

	//Remove copy constructor functionality
	Renderer(const Renderer& renderer);
	Renderer& operator=(const Renderer& renderer);

	//Fetch Functions
	Program* GetProgram(const char* programName);
	Shader* GetShader(const char* shaderName);
	Texture* GetTexture(const char* TextureFilename);
	Model* GetModel(const char* ModelFilename);
	TextGenerator* GetTextGenerator(std::string FontFilename, unsigned int fontSize);

	//OpenGl Creation Functions
	Shader& CreateShader(GLenum shaderType, const char* shaderName);
	GLuint CreateVBO(GLfloat* data, GLsizeiptr size);
	GLuint CreateEBO(GLuint* data, GLsizeiptr size);

	//Error Printing Functions
	std::string ReadShaderFile(const char* filename);
	void PrintErrorDetails(bool isShader, GLuint id, const char* name);
	void CheckGLErrors();

	//VAO Creation Functions
	GLuint CreateCircleVAO(const unsigned int stacks);
	GLuint CreateSphereVAO(const unsigned int stacks, const unsigned int sectors);

//Member Data
public:
	static int SCREEN_WIDTH;
	static int SCREEN_HEIGHT;
	static glm::mat4 SCREEN_PROJ;

protected:
	static Renderer* sm_pInstance;

private:
	std::map<std::string, Program*> m_programs;
	std::map<std::string, Shader*> m_shaders;

	//std::map<std::string, GLuint> m_VAOLibrary;

	std::map<std::string, Mesh*> m_pMeshLibrary;
	std::map<std::string, Texture*> m_pTextures;
	std::map<std::string, TextGenerator*> m_pTextGenerators;
	std::map<std::string, Model*> m_pModels;
};

#endif // !_RENDERER_H_