#ifndef _SHADER_H_
#define _SHADER_H_

#include "Renderer.h"

class Shader
{
public:
	Shader(GLenum shaderType, const char* shaderName);
	~Shader() {};

	GLuint GetID() { return m_shaderID; };
	const char* GetName() {return m_shaderName.c_str();};
protected:
	void PrintErrorDetails();
	std::string ReadFile(const char* filename);
private:

public:

protected:
	GLenum m_shaderType;
	std::string m_shaderName;
	GLuint m_shaderID;
private:

};

#endif