// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// ? 2021 Media Design School
//
// Filename: SceneManager.cpp
// Description: Singleton class incharge of handling the scene transitions.
// Author(s): Michael Jordan
// 

#include "SceneManager.h"
#include "InputHandler.h"
#include "GlobalVariables.h"
#include "Scene.h"
#include "PauseMenuScene.h"
#include "Entity.h"
#include <iostream>

//Statics:
SceneManager* SceneManager::sm_pInstance = nullptr;

// Name: GetInstance
// Author: Michael Jordan
// Description: Get the single instance of the scene manager.
// 
// Param:
//   - void
// Return: 
//   SceneManager& : "Current single instance of the scene manager."

SceneManager& SceneManager::GetInstance()
{
	if (sm_pInstance == nullptr)
		sm_pInstance = new SceneManager();

	return *sm_pInstance;
}

// Name: DestroyInstance
// Author: Michael Jordan
// Description: Calls the deconstructor of the single instance of the scene manager.
// 
// Param:
//   - void
// Return: 
//   void

void SceneManager::DestroyInstance()
{
	if (sm_pInstance != nullptr)
		delete sm_pInstance;

	sm_pInstance = nullptr;
}

//Contructor
SceneManager::SceneManager()
	: m_topScene(nullptr), m_nextScene(nullptr), m_pauseScene(nullptr)
{
	m_isRunning = false;
}

//Destructor
SceneManager::~SceneManager()
{
	delete m_topScene;
	m_topScene = nullptr;
	delete m_nextScene;
	m_nextScene = nullptr;
}

void SceneManager::DestroyEntity(Entity* entity)
{
	m_topScene->DestroyEntity(entity);
}

// Name: DoSceneLoop
// Author: Michael Jordan
// Description: Starts the game loop which is contained within it's own while loop.
//     The loop will only close once the quit function is called.
// Param:
//   - void
// Return: 
//   void
void SceneManager::DoSceneLoop()
{
	float lag = 0.0f;

	if (!m_isRunning) //Safety check for multiple scene loops.
	{
		m_isRunning = true;
		m_clock.restart();
		do {
			//Calculate time since last loop
			float deltaTime = m_clock.getElapsedTime().asSeconds();
			C_GlobalVariables::DeltaTime = deltaTime; // Sonja

			m_clock.restart();

			InputHandler::GetInstance().Update();

			//Compensate for lag
			lag += deltaTime;

			if (lag > 1.0f / sm_totalFrames) 
			{
				if (pauseDelay > 0)
					pauseDelay -= 1.0f / sm_totalFrames;

				lag -= 1.0f / sm_totalFrames;

				if (m_topScene != nullptr)
				{
					Renderer::GetInstance().Clear();

					//Update by a fixed time
					m_topScene->Update(1.0f / sm_totalFrames);

					//Render the frame
					m_topScene->Draw();

					Renderer::GetInstance().Display();
				}

				//Transition to the next scene
				LoadNextScene();
			}
		} while (m_isRunning);
	}
}

void SceneManager::PauseScene()
{
	if (pauseDelay <= 0.0f)
	{
		if (m_pauseScene == nullptr)
			m_pauseScene = new PauseMenuScene(m_topScene);
		pauseDelay = 1.0f;
	}
}

void SceneManager::UnPauseScene()
{
	if(pauseDelay <= 0.0f)
	{
		m_nextScene = m_pauseScene->GetUnderScene();
		pauseDelay = 1.0f;
	}
}

// Name: LoadNextScene
// Author: Michael Jordan
// Description: Transitions to the next scene iff another scene has been loaded by the
//     TransitionTo function. This function is called at the end of each frame.
// Param:
//   - void
// Return: 
//   void
void SceneManager::LoadNextScene()
{
	if (m_pauseScene != nullptr)
	{
		if (m_pauseScene != m_topScene)
		{
			m_topScene = m_pauseScene;
			return;
		}
		if (m_nextScene != nullptr)
		{
			m_pauseScene = nullptr;
		}
	}

	if (m_nextScene != nullptr)
	{
		delete m_topScene; //Delete current scene
		m_topScene = m_nextScene; //Switch
		m_nextScene = nullptr; //Remove next scene reference
	}
}
