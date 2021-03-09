#version 450 core
layout (location = 0) in vec3 position;

out vec3 fragTexCoord;

uniform mat4 MVP; 
uniform mat4 transform;

void main()
{
	gl_Position = MVP * transform * vec4(position, 1.0f);
	fragTexCoord = position;
}