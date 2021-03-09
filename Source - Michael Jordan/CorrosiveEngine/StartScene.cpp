//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	StartScene.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	StartScene
// Description:
//		Starting scene on a timer, to display the engine logo.
//

//Self Includes
#include "StartScene.h"

//Local Includes
#include "MainMenuScene.h"
//#include "GameScene.h"
#include "SceneManager.h"
#include "AudioManager.h"
#include "MeshEntity.h"
#include "Program.h"

//Constructor
StartScene::StartScene()
{
	
}

//Destructor
StartScene::~StartScene()
{
	LightingSystem::GetInstance().ClearLights();
	m_light = 0;

	m_logo = 0;
}

bool StartScene::Initialise()
{
	m_logo = new MeshEntity(Renderer::GetInstance().CreateSquareMesh());
	m_logo->SetTexture(Renderer::GetInstance().CreateTexture("Logo.png"));
	m_logo->GetTransformRef().GetScaleRef() = glm::vec3(0.5f, 0.5f, 1.0f);
	InstantiateEntity(m_logo);

	//Play start music
	AudioManager::GetInstance().CreateSound("StartMusic.wav", FMOD_DEFAULT);
	AudioManager::GetInstance().PlaySoundEffect("StartMusic.wav");

	m_logoAlpha = 1.0f;

	m_light = LightingSystem::GetInstance().CreatePointLight(glm::vec3(-3.0f, 2.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	LightingSystem::GetInstance().AddPointLight(m_light);

	Process(0.0f);

	return true;
}

void StartScene::PreLoad()
{

}

//	Draw( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Draws all entities and objects within the scene.
//		
//	Param:
//		- n/a	|	
//
//	Return: n/a		|
//
void StartScene::Draw()
{
	m_pProgram->Use();
	m_pProgram->SetVector3("camPos", glm::vec3(1.0f, 0.0f, 0.0f));

	DrawEntities(nullptr);

	m_pProgram->Clear();
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes all entities and objects within the scene since the last call.
//		
//	Param:
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void StartScene::Process(float dT)
{
	m_logoAlpha -= dT / 1.5f;

	ProcessEntities(dT);

	m_logo->SetColour(glm::vec4(1.0f, 1.0f, 1.0f, m_logoAlpha));

	m_light->position.y -= 0.05f;

	if (m_logoAlpha < 0.0f) 
	{
		//End scene
		SceneManager::PopScene();
		SceneManager::PushScene(new MainMenuScene(), true);
	}
}

//	Resize( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Called via the scene manager to alert that the window has resized.
//		
//	Param:
//		- n/a	|	
//
//	Return: n/a		|
//
void StartScene::Resize()
{
	//Do Nothing
}
