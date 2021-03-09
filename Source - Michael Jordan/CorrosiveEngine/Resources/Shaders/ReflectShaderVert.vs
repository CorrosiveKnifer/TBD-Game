#version 450 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords; //Frame size

//Matrices
uniform mat4 MVP; 
uniform mat4 transform; 

//Texture UV manipulation
uniform vec2 texOffset		=	vec2(0.0f, 0.0f);	//Start loc for anim frames
uniform vec2 frame			=	vec2(1, 1);			//Skip to frame (start = 1)

out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoord;

void main()
{
	gl_Position = MVP * transform * vec4(position, 1.0f);

	fragNormal = mat3(transpose(inverse(transform))) * normal;
	fragPos = vec3(transform * vec4(position, 1.0f));
	fragTexCoord.x = texCoords.x * frame.x + texOffset.x;
	fragTexCoord.y = texCoords.y * frame.y + texOffset.y;
}