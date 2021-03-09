#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 finalColor;

//Image information
uniform sampler2D imageTexture;
uniform vec4 imageColour	=	vec4(1.0f, 1.0f, 1.0f, 1.0f);

//Lighting (Phong)
uniform float ambStrength	=	0.05f;
uniform vec3 ambColour		=	vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightColour	=	vec3(1.0f, 1.0f, 1.0f);
uniform vec3 lightPos		=	vec3(1.0f, 0.0f, 0.0f);

uniform vec3 camPos			=	vec3(1.0f, 0.0f, 0.0f);
uniform float specStrength	=	1.0f;
uniform float shininess		=	32.0f;

//Rim Color
uniform int rimExponent		=	1;
uniform vec3 rimColor		=	vec3(1.0f, 0.0f, 0.0f);

void main()
{
	//Ambient
	vec3 ambient = ambStrength * ambColour;

	//Light Direction
	vec3 norm = normalize(fragNormal);
	vec3 lightDir = normalize(fragPos - lightPos);

	//Diffuse Colouring
	float diffuseStrength = max(dot(norm, -lightDir), 0.0f);
	vec3 diffuse = diffuseStrength * lightColour;

	//Specular Highlight
	vec3 negViewDir = normalize(camPos - fragPos);
	vec3 reflectDir = reflect(lightDir, norm);
	float spec = pow(max(dot(negViewDir, reflectDir), 0.0f), shininess);
	vec3 specular = specStrength * spec * lightColour;

	//rimColor
	float rimFactor = 1.0 - dot(norm, negViewDir);
	rimFactor = smoothstep(0.0f, 1.0f, rimFactor);
	rimFactor = pow(rimFactor, rimExponent);
	vec3 rim = rimFactor * rimColor * lightColour;

	finalColor = vec4(ambient + diffuse + specular + rim, 1.0f) * texture(imageTexture, fragTexCoord) * imageColour;
}