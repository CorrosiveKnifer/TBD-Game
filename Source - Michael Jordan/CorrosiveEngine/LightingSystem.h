#ifndef _LIGHTING_SYSTEM_H_
#define _LIGHTING_SYSTEM_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	LightingSystem.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	LightingSystem
// Description:
//		Light container used to delegate which light to use based on the distance
//		of the object using the shader.
//

//Library Includes
#include <vector>

//Local Includes
#include "Program.h"

//Base Light struct
struct Light
{
	glm::vec3 position;
	glm::vec3 color;
};

//Directional light struct
struct DirectLight : Light
{
	glm::vec3 direction;

	//Color * strength
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

//Point Light struct
struct PointLight : Light
{
	float constant;
	float linear;
	float quadratic;

	//Color * strength
	glm::vec3 ambient; 
	glm::vec3 diffuse; 
	glm::vec3 specular;
};

//Spot Light struct
struct SpotLight : DirectLight
{
	//in "cos(radians)"
	float innerCutOff;
	float outerCutOff;

	//Range
	float constant;
	float linear;
	float quadratic;
};

class LightingSystem
{
	//Member Functions:
public:
	static LightingSystem& GetInstance();
	static void DestroyInstance();

	unsigned int GetPointLightCount() { return m_pPointLights.size(); };

	void AddDirectLight(DirectLight* _light);
	void AddPointLight(PointLight* _light) { m_pPointLights.push_back(_light); };
	void AddSpotLight(SpotLight* _light) { m_pSpotLights.push_back(_light); };

	DirectLight* CreateDirectLight(glm::vec3 direction, glm::vec3 color);
	PointLight* CreatePointLight(glm::vec3 position, glm::vec3 color);
	SpotLight* CreateSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float InCut = 25.0f, float OutCut = 35.0f);
	
	void ApplyLights(Program* program, glm::vec3 _target, float distance = -1);
	void ClearLights();
protected:
	
private:
	LightingSystem();
	~LightingSystem();
	//Member Data:
public:

protected:

private:
	static LightingSystem* m_spInstance;

	std::vector<PointLight*> m_pPointLights;
	std::vector<SpotLight*> m_pSpotLights;
	DirectLight* m_mainLight;
};


#endif // !_LIGHTING_SYSTEM_H_


