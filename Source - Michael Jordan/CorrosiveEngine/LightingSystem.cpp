//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	LightingSystem.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	LightingSystem
// Description:
//		Light container used to delegate which light to use based on the distance
//		of the object using the shader.
//

//Self Include
#include "LightingSystem.h"

//Static variabes
LightingSystem* LightingSystem::m_spInstance = 0;

//Constructor
LightingSystem::LightingSystem()
{
	m_mainLight = nullptr;
}

//Destructor
LightingSystem::~LightingSystem()
{
	ClearLights(); //Frees all memory associated to lights
}

//	GetInstance( )
//
//	Author: Michael Jordan
//	Access: public 
//	Description:
//		Get function for the only instance of this class. If the instance doesn't
//		exist then it will make one.
//		
//	Param:
//		- n/a
//
//	Return: LightingSystem&	|	Single instance of the Renderer
//
LightingSystem& LightingSystem::GetInstance()
{
	if (m_spInstance == 0)
	{
		m_spInstance = new LightingSystem();
	}
	return *m_spInstance;
}

//	DestroyInstance( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Destroys the single instance of this class
//		
//	Param:
//		- n/a
//
//	Return: n/a		|
//
void LightingSystem::DestroyInstance()
{
	if (m_spInstance != 0)
	{
		delete m_spInstance;
		m_spInstance = 0;
	}
}

//	AddDirectLight( _light )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Replaces the current directional light with the one provided.
//		Note: Directional lights should not be used until shadows are implemented.
//		
//	Param:
//		-	DirectLight*	|	new directional light to add to the system.
//
//	Return: n/a		|
//
void LightingSystem::AddDirectLight(DirectLight* _light)
{
	if (m_mainLight != nullptr)
	{
		delete m_mainLight;
	}
	m_mainLight = _light;
}

//	AddDirectLight( direction, color )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates a pre-designed directional light based on the direction and color provided.
//		
//	Param:
//		-	glm::vec3	|	Direction of the light.
//		-	glm::vec3	|	Color of the light.
//
//	Return: DirectLight*	|	new directional light.
//
DirectLight* LightingSystem::CreateDirectLight(glm::vec3 direction, glm::vec3 color)
{
	DirectLight* temp = new DirectLight();
	temp->position = glm::vec3(0.0f, 0.0f, 0.0f);
	temp->direction = direction;

	//Color * strength
	temp->ambient = 0.05f * color;
	temp->diffuse = color;
	temp->specular = 1.0f * color;
	temp->color = color;
	return temp;
}

//	CreatePointLight( position, color)
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates a pre-designed point light based on the position and color provided.
//		
//	Param:
//		-	glm::vec3	|	Position of the light.
//		-	glm::vec3	|	Color of the light.
//
//	Return: PointLight*	|	new point light.
//
PointLight* LightingSystem::CreatePointLight(glm::vec3 position, glm::vec3 color)
{
	PointLight* temp = new PointLight();
	temp->position = position;
	temp->color = color;

	//Color * strength
	temp->ambient = 0.05f * color;
	temp->diffuse = color;
	temp->specular = 1.0f * color;
	
	//Range
	temp->constant = 1.0f;
	temp->linear = 0.045f;
	temp->quadratic = 0.0075f;

	return temp;
}

//	CreateSpotLight( position, direction, color, InCut, OutCut )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates a pre-designed spot light based on the position, direction and color provided.
//		Cut off angle are based when the light intensitiy starts and stops fading. 
//		
//	Param:
//		-	glm::vec3	|	Position of the light.
//		-	glm::vec3	|	Direction of the light.
//		-	glm::vec3	|	Color of the light.
//		-	float		|	internal cut off angle (in degrees)(default = 25').
//		-	float		|	external cut off angle (in degrees)(default = 35').
//
//	Return: SpotLight*	|	new spot light.
//
SpotLight* LightingSystem::CreateSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color, float InCut, float OutCut)
{
	SpotLight* temp = new SpotLight();
	temp->position = position;
	temp->direction = direction;
	temp->color = color;
	//Color * strength
	temp->ambient = 0.05f * color;
	temp->diffuse = color;
	temp->specular = 1.0f * color;

	//Range
	temp->constant = 1.0f;
	temp->linear = 0.045f;
	temp->quadratic = 0.0075f;

	//Cutoffs
	temp->innerCutOff = glm::cos(glm::radians(InCut));
	temp->outerCutOff = glm::cos(glm::radians(OutCut));

	return temp;
}

//	CreateSpotLight( program, _target, distance )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Applies all lighting information into the program for the main 
//		directional light, 10 of the closest point lights and 10 of the 
//		closest spot lights.
//		
//	Param:
//		-	GLuint		|	Program to apply to.
//		-	glm::vec3	|	Position of the target being lit up.
//		-	float		|	distance away from the target (default = -1 AKA infinite distance).
//
//	Return: n/a		|
//
void LightingSystem::ApplyLights(Program* program, glm::vec3 _target, float distance)
{
	if (m_pPointLights.size() == 0 && m_mainLight == nullptr)
	{
		program->SetInteger("pointLightCount", 0);
		program->SetInteger("spotLightCount", 0);
		return;
	}
		

	PointLight* closestPoints[10];
	SpotLight* closestSpots[10];

	//Get All closest point lights
	unsigned int pointLightCount = 0;
	for (unsigned int i = 0; i < m_pPointLights.size(); i++)
	{
		//Calculate distance
		float distToLight = glm::distance(_target, m_pPointLights.at(i)->position);
		
		if (distance < 0 || distToLight < distance)
		{
			//Light is within range
			closestPoints[pointLightCount] = m_pPointLights.at(i);
			
			pointLightCount++;
			if (pointLightCount == 10) 
				break; //Stop looking once at 10
		}
	}

	//Get All closest spotlights
	unsigned int spotLightCount = 0;
	for (unsigned int i = 0; i < m_pSpotLights.size(); i++)
	{
		//Calculate distance
		float distToLight = glm::distance(_target, m_pSpotLights.at(i)->position);

		if (distance < 0 || distToLight < distance)
		{
			//Light is within range
			closestSpots[spotLightCount] = m_pSpotLights.at(i);

			spotLightCount++;
			if (spotLightCount == 10)
				break; //Stop looking once at 10
		}
	}

	//Apply point lights
	program->SetInteger("pointLightCount", pointLightCount);
	for (unsigned int i = 0; i < pointLightCount; i++)
	{
		std::string loc = "pointLights[" + std::to_string(i) + "]";
		program->SetVector3((loc + ".position").c_str(), closestPoints[i]->position); //pointLights[0].position

		program->SetFloat((loc + ".constant").c_str(), closestPoints[i]->constant);
		program->SetFloat((loc + ".linear").c_str(), closestPoints[i]->linear);
		program->SetFloat((loc + ".quadratic").c_str(), closestPoints[i]->quadratic);

		program->SetVector3((loc + ".ambient").c_str(), closestPoints[i]->ambient);
		program->SetVector3((loc + ".diffuse").c_str(), closestPoints[i]->diffuse);
		program->SetVector3((loc + ".specular").c_str(), closestPoints[i]->specular);
	}

	//Apply the single directional light
	if (m_mainLight != nullptr)
	{
		program->SetVector3("mainDirectLight.direction", m_mainLight->direction);

		program->SetVector3("mainDirectLight.ambient", m_mainLight->ambient);
		program->SetVector3("mainDirectLight.diffuse", m_mainLight->diffuse);
		program->SetVector3("mainDirectLight.specular", m_mainLight->specular);
	}

	//Apply spot lights
	program->SetInteger("spotLightCount", spotLightCount);
	for (unsigned int i = 0; i < spotLightCount; i++)
	{
		std::string loc = "spotLights[" + std::to_string(i) + "]";
		program->SetVector3((loc + ".position").c_str(), closestSpots[i]->position);
		program->SetVector3((loc + ".direction").c_str(), closestSpots[i]->direction);

		program->SetFloat((loc + ".innerCutOff").c_str(), closestSpots[i]->innerCutOff);
		program->SetFloat((loc + ".outerCutOff").c_str(), closestSpots[i]->outerCutOff);

		program->SetFloat((loc + ".constant").c_str(), closestSpots[i]->constant);
		program->SetFloat((loc + ".linear").c_str(), closestSpots[i]->linear);
		program->SetFloat((loc + ".quadratic").c_str(), closestSpots[i]->quadratic);

		program->SetVector3((loc + ".ambient").c_str(), closestSpots[i]->ambient);
		program->SetVector3((loc + ".diffuse").c_str(), closestSpots[i]->diffuse);
		program->SetVector3((loc + ".specular").c_str(), closestSpots[i]->specular);
	}
}

//	ClearLights( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Frees the memory associated with all lights within the system.
//		
//	Param:
//		-	n/a		|
//
//	Return: n/a		|
//
void LightingSystem::ClearLights()
{
	//Clear point lights
	std::vector<PointLight*>::iterator pIter = m_pPointLights.begin();
	while (pIter != m_pPointLights.end())
	{
		delete * pIter;
		pIter++;
	}
	m_pPointLights.clear();

	//Clear Spotlights
	std::vector<SpotLight*>::iterator sIter = m_pSpotLights.begin();
	while (sIter != m_pSpotLights.end())
	{
		delete* sIter;
		sIter++;
	}
	m_pSpotLights.clear();

	//Clear Directional Light
	delete m_mainLight;
	m_mainLight = nullptr;
}
