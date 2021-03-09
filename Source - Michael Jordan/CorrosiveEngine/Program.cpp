#include "Program.h"
#include "Program.h"

#include <vector>
#include <iostream>

Program::Program(GLuint _id, std::string _name)
{
	m_programID = _id;
	m_programName = _name;
}

Program::Program(Shader* _vertexShader, Shader* _fragmentShader)
{
	m_programID = glCreateProgram();

	m_programName = "";
	m_programName += _vertexShader->GetName();
	m_programName += " | ";
	m_programName += _fragmentShader->GetName();

	glAttachShader(m_programID, _vertexShader->GetID());
	glAttachShader(m_programID, _fragmentShader->GetID());
	glLinkProgram(m_programID);

	//Check any linking errors
	int link_result = 0;
	glGetProgramiv(m_programID, GL_LINK_STATUS, &link_result);

	if (link_result == GL_FALSE)
	{
		PrintErrorDetails();
	}
}

void Program::Clear()
{
	glBindVertexArray(0);
	glUseProgram(0);
}

void Program::SetFloat(const char* floatName, float value)
{
	glUniform1f(glGetUniformLocation(m_programID, floatName), value);
}

void Program::SetInteger(const char* integerName, int value)
{
	glUniform1i(glGetUniformLocation(m_programID, integerName), value);
}

void Program::SetVector2(const char* vectorName, glm::vec2 value)
{
	glUniform2fv(glGetUniformLocation(m_programID, vectorName), 1, glm::value_ptr(value));
}

void Program::SetVector3(const char* vectorName, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(m_programID, vectorName), 1, glm::value_ptr(value));
}

void Program::SetVector4(const char* vectorName, glm::vec4 value)
{
	glUniform4fv(glGetUniformLocation(m_programID, vectorName), 1, glm::value_ptr(value));
}

void Program::SetMatrix2(const char* matrixName, glm::mat2 value)
{
	glUniformMatrix2fv(glGetUniformLocation(m_programID, matrixName), 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetMatrix3(const char* matrixName, glm::mat3 value)
{
	glUniformMatrix3fv(glGetUniformLocation(m_programID, matrixName), 1, GL_FALSE, glm::value_ptr(value));
}

void Program::SetMatrix4(const char* matrixName, glm::mat4 value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_programID, matrixName), 1, GL_FALSE, glm::value_ptr(value));
}

void Program::BindVertextArray(GLuint _vao)
{
	glBindVertexArray(_vao);
}

void Program::PrintErrorDetails()
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	glGetProgramiv(m_programID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> log(infoLogLength);

	// Retrieve the log info and populate log variable
	glGetProgramInfoLog(m_programID, infoLogLength, NULL, &log[0]);
	std::cout << "Error compiling " << "program" << ": " << m_programName << std::endl;
	std::cout << &log[0] << std::endl;
}
