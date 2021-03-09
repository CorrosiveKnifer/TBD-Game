#version 450 core

layout (location = 0) in vec3 Position;
layout (location = 1) in vec3 Normals;
layout (location = 2) in vec2 TexCoords;

//Matrix information
uniform mat4 CameraMat;
uniform mat4 Transform;

uniform vec4 Color;

out vec4 FragColor;
out vec2 FragTexCoords;

void main()
{
	gl_Position = CameraMat * Transform * vec4(Position, 1.0f);

	FragColor = Color;

	FragTexCoords = TexCoords;
}