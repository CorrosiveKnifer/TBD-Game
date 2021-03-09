#include "Shader.h"
#include <iostream>
#include <fstream>
#include <vector>

Shader::Shader(GLenum shaderType, const char* shaderName)
{
	std::string filename = "Resources/Shaders/";
	filename += shaderName;

	std::string file = ReadFile(filename.c_str());
	const GLchar* c_str = file.c_str();
	const GLint size = file.size();

	m_shaderType = shaderType;
	m_shaderID = glCreateShader(m_shaderType);
	m_shaderName = shaderName;

	glShaderSource(m_shaderID, 1, &c_str, &size);
	glCompileShader(m_shaderID);

	int compile_result = 0;
	glGetShaderiv(m_shaderID, GL_COMPILE_STATUS, &compile_result);
	if (compile_result == GL_FALSE)
	{
		PrintErrorDetails();
	}
}

void Shader::PrintErrorDetails()
{
	int infoLogLength = 0;
	// Retrieve the length of characters needed to contain the info log
	glGetShaderiv(m_shaderID, GL_INFO_LOG_LENGTH, &infoLogLength);
	std::vector<char> log(infoLogLength);

	// Retrieve the log info and populate log variable
	glGetShaderInfoLog(m_shaderID, infoLogLength, NULL, &log[0]);
	std::cout << "Error compiling " << "shader" << ": " << m_shaderName << std::endl;
	std::cout << &log[0] << std::endl;
}

std::string Shader::ReadFile(const char* filename)
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
