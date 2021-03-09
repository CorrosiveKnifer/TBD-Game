#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 finalColor;

uniform vec3 camPos;
uniform sampler2D tex;
uniform samplerCube skybox;

void main()
{
	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(fragPos - camPos);
	vec3 reflectDir = reflect(viewDir, norm);
	finalColor = vec4(texture(skybox, reflectDir).rgb, 1.0f);
}