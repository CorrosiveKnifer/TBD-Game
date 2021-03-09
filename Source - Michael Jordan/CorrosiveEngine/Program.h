#ifndef _PROGRAM_H_
#define _PROGRAM_H_

#include "Renderer.h"
#include "Shader.h"

class Program
{

public:
	Program(GLuint _id, std::string name);
	Program(Shader* _vertexShader, Shader* _fragmentShader);
	~Program() {};

	void Use() { glUseProgram(m_programID); };
	void Clear();

	void SetFloat(const char* floatName, float value);
	void SetInteger(const char* integerName, int value);

	void SetVector2(const char* vectorName, glm::vec2 value);
	void SetVector3(const char* vectorName, glm::vec3 value);
	void SetVector4(const char* vectorName, glm::vec4 value);

	void SetMatrix2(const char* matrixName, glm::mat2 value);
	void SetMatrix3(const char* matrixName, glm::mat3 value);
	void SetMatrix4(const char* matrixName, glm::mat4 value);

	void BindVertextArray(GLuint _vao);
protected:
	void PrintErrorDetails();
private:

public:

protected:
	std::string m_programName;
	GLuint m_programID;
private:

};

#endif
