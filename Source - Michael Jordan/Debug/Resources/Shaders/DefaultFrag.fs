#version 450 core

in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoord;

out vec4 finalColor;

//Image information
uniform sampler2D imageTexture;
uniform vec4 imageColour	=	vec4(1.0f, 1.0f, 1.0f, 1.0f);

uniform vec3 camPos			=	vec3(1.0f, 0.0f, 0.0f);
uniform float shininess		=	32.0f;

uniform bool showRim		=	false;
uniform int rimExponent		=	4;
uniform vec3 rimColor		=	vec3(1.0f, 1.0f, 1.0f);

struct PointLight
{
	vec3 position;

	//Range
	float constant;
	float linear;
	float quadratic;

	//strength * colour
	vec3 ambient;	
	vec3 diffuse;
	vec3 specular;	
};

struct DirectLight
{
	vec3 direction;

	//strength * colour
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct SpotLight
{
	vec3 position;
	vec3 direction;

	//in "cos(radians)"
	float innerCutOff;
	float outerCutOff; 

	//Range
	float constant;
    float linear;
    float quadratic;

	//strength * colour
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

//Lighting Uniform
uniform int pointLightCount = 10;
uniform int spotLightCount = 10;
uniform PointLight pointLights[10];
uniform DirectLight mainDirectLight;
uniform SpotLight spotLights[10];

vec3 CalculateDirectLight(DirectLight dLight, vec3 norm, vec3 viewDir)
{
	vec3 lightDir = normalize(-dLight.direction);

	//Diffuse shading
	float diffuseStrength = max(dot(norm, lightDir), 0.0f);

	//Specular Shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);

	//Calculate light vectors
	vec3 ambient = dLight.ambient;
	vec3 diffuse = dLight.diffuse * diffuseStrength;
	vec3 specular = dLight.specular * spec;

	return (ambient + diffuse + specular);
}

vec3 CalculatePointLight(PointLight pLight, vec3 norm, vec3 viewDir)
{
	vec3 lightDir = normalize(pLight.position - fragPos);

	//Diffuse shading
	float diffuseStrength = max(dot(norm, lightDir), 0.0f);

	//Specular Shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);

	float distance = length(pLight.position - fragPos);
	float denom = pLight.constant + pLight.linear * distance + pLight.quadratic * (distance * distance);
	float attenuation = 1.0f / denom;

	//Calculate light vectors
	vec3 ambient = pLight.ambient;
	vec3 diffuse = pLight.diffuse * diffuseStrength;
	vec3 specular = pLight.specular * spec;

	//Combine the result
	return (ambient + diffuse + specular) * attenuation;
}

vec3 CalculateSpotLight(SpotLight sLight, vec3 norm, vec3 viewDir)
{
	vec3 lightDir = normalize(sLight.position - fragPos);

	//Diffuse shading
	float diffuseStrength = max(dot(norm, lightDir), 0.0f);

	//Specular Shading
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0f), shininess);

	//Angle between "direction to the fragment" and "light's direction"
	float theta = dot(lightDir, normalize(-sLight.direction));

	//intensity calculation when inbetween the cuttoff ranges:
	float epsilon = sLight.innerCutOff - sLight.outerCutOff;
	float intensity = clamp((theta - sLight.outerCutOff) / epsilon, 0.0f, 1.0f); //1.0f = full bright

	if(theta > sLight.outerCutOff)
	{
		//Light Up
		vec3 ambient = sLight.ambient;
		vec3 diffuse = sLight.diffuse * diffuseStrength;
		vec3 specular =  sLight.specular * spec;

		//Distance Scale:
		float distance = length(sLight.position - fragPos);
		float denom = sLight.constant + sLight.linear * distance + sLight.quadratic * (distance * distance);
		float attenuation = 1.0f / denom;

		return (ambient + diffuse + specular) * attenuation * intensity;
	}
	else
		return vec3(0.0f); //No light
}

void main()
{
	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(camPos - fragPos);
	vec3 light = CalculateDirectLight(mainDirectLight, norm, viewDir);

	for(int i = 0; i < pointLightCount; i++)
	{
		light += CalculatePointLight(pointLights[i], norm, viewDir);
	}

	for(int i = 0; i < spotLightCount; i++)
	{
		light += CalculateSpotLight(spotLights[i], norm, viewDir);
	}

	if(showRim)
	{
		float rimFactor = 1.0 - dot(norm, normalize(camPos - fragPos));
		rimFactor = smoothstep(0.0f, 1.0f, rimFactor);
		rimFactor = pow(rimFactor, rimExponent);
		vec3 rim = rimFactor * rimColor * vec3(1.0f, 1.0f, 1.0f);
		light += rim;
	}

	finalColor = vec4(light, 1.0f) * texture(imageTexture, fragTexCoord) * imageColour;
}