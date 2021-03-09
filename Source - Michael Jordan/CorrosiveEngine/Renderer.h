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

	Program& CreateProgram(const char* VertexShaderFilename, const char* FragmentShaderFilename);
	GLuint CreateVAO(uint32_t type, GLsizeiptr vertCount, GLsizeiptr indCount, GLfloat* vertData, GLuint* indData);
	Texture& CreateTexture(const char* TextureFilename);
	Texture& CreateCubeMapTexture(const char* _title, std::string _textureNames[6]);

	Model& CreateModel(const char* ModelFilename);
	Mesh& CreateSquareMesh();
	Mesh& CreateCubeMesh();
	Mesh& CreateSphereMesh(unsigned int stacks, unsigned int sectors);

	TextGenerator& CreateTextGenerator(std::string font, unsigned int fontSize);

	//Defaults
	Program& CreateDefaultProgram();
	GLuint CreateDefaultVAO(GLsizeiptr vertCount, GLsizeiptr indCount, GLfloat* vertData, GLuint* indData);
	
	Mesh& CreateCubeMapMesh();

	static unsigned int GetVectorSize(uint32_t type);

	glm::vec3 GetMouseDirectionToWorld(Camera* camera);
	glm::vec3 GetPositonFromRayOnPlane(glm::vec3 origin, glm::vec3 direction, glm::vec3 norm, float dist = 0);
	
	void DrawPoint(Camera* camera, glm::vec3 point, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f));
	void DrawLine(Camera* camera, glm::vec3 origin, glm::vec3 direction, float distance, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f), float thickness = 1.0f);
	void DrawLineBetween(Camera* camera, glm::vec3 pointA, glm::vec3 pointB, glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f), float thickness = 1.0f);
protected:
	
private:
	Renderer();
	~Renderer();

	//Remove copy constructor
	Renderer(const Renderer& renderer);
	Renderer& operator=(const Renderer& renderer);

	void CheckGLErrors();

	Program* GetProgram(const char* programName);
	Shader* GetShader(const char* shaderName);
	Texture* GetTexture(const char* TextureFilename);
	Model* GetModel(const char* ModelFilename);
	TextGenerator* GetTextGenerator(std::string FontFilename, unsigned int fontSize);

	Shader& CreateShader(GLenum shaderType, const char* shaderName);
	GLuint CreateVBO(GLfloat* data, GLsizeiptr size);
	GLuint CreateEBO(GLuint* data, GLsizeiptr size);

	std::string ReadShaderFile(const char* filename);
	void PrintErrorDetails(bool isShader, GLuint id, const char* name);

	GLuint CreateSphereVAO(const unsigned int stacks, const unsigned int sectors);
	//Member Data:
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