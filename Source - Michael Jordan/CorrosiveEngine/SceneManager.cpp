//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	SceneManager.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	SceneManager
// Description:
//		Graphical Library Manager that contains static functions to call
//		SceneADT functions. This class also stores the Scene stack and is incharge of
//		switching/freeing scenes.
//

//Parent Include
#include "SceneManager.h"

//Local Includes
#include "InputHandler.h"
#include "AudioManager.h"
#include "Renderer.h"

//#include "GameScene.h"

#include <Thread>

//Static variables
float SceneManager::sm_prevTime = 0.0f;
std::stack<SceneADT*> SceneManager::m_pScenesStack;
SceneADT* SceneManager::nextScene = nullptr;

//	PushScene( topScene)
//
//	Author: Michael Jordan
//	Description:
//		Loads a the topScene provided onto the stack.
//		GLManager takes ownership of the scene provided.
//		
//	Param:
//		- SceneADT*	|	Pointer to the new top scene.
//
//	Return: n/a		|	
//
void SceneManager::PushScene(SceneADT* topScene, bool _willInitialise)
{
	if (_willInitialise)
		topScene->Initialise();

	m_pScenesStack.push(topScene);
	sm_prevTime = (float)glutGet(GLUT_ELAPSED_TIME) * 0.001f;
}

//	PopScene()
//
//	Author: Michael Jordan
//	Description:
//		Pops the top scene off the stack and frees it's memory.
//		
//	Param:
//		
//	Return: n/a		|	
//
SceneADT* SceneManager::PopScene(bool willDelete)
{
	SceneADT* temp = m_pScenesStack.top();
	m_pScenesStack.pop();

	if (willDelete)
	{
		delete temp; //Clear memory

		sm_prevTime = (float)glutGet(GLUT_ELAPSED_TIME) * 0.001f;
		return 0;
	}
	return temp;
}

void SceneManager::InstantiateEntity(Entity* entity)
{
	m_pScenesStack.top()->InstantiateEntity(entity);
}

void SceneManager::DestroyEntity(Entity* entity, bool toDeconstruct)
{
	m_pScenesStack.top()->DestroyEntity(entity, toDeconstruct);
}

void SceneManager::LoadScene(SceneADT* _newScene)
{
	//Push load scene to the stack
	PushScene(new LoadScreenScene(_newScene), true);
}

//	TopScene()
//
//	Author: Michael Jordan
//	Description:
//		Gets the top scene on the stack.
//		
//	Param:
//		
//	Return: n/a		|	
//
SceneADT* SceneManager::TopScene()
{
	return m_pScenesStack.top();
}

//	EmptyScenes()
//
//	Author: Michael Jordan
//	Description:
//		Removes and frees all scenes on the stack.
//		
//	Param:
//		
//	Return: n/a		|	
//
void SceneManager::EmptyScenes()
{
	while (!m_pScenesStack.empty())
	{
		SceneADT* temp = m_pScenesStack.top();
		m_pScenesStack.pop();
		delete temp; //Clear memory
	}
	std::stack<SceneADT*>().swap(m_pScenesStack);
}

//	Draw()
//
//	Author: Michael Jordan
//	Description:
//		Function in charge of rendering to the back buffer and switching buffers.
//		
//	Param:
//		
//	Return: n/a		|	
//
void SceneManager::Draw()
{
	//Clear BackBuffer
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

	if (!m_pScenesStack.empty())
		m_pScenesStack.top()->Draw();

	//Switch Buffer
	glutSwapBuffers();
}

//	Process()
//
//	Author: Michael Jordan
//	Description:
//		Calculates the time since the last call and processes the next frame.
//		
//	Param:
//		
//	Return: n/a		|	
//
void SceneManager::Process()
{
	glutPostRedisplay();

	float currentTime = (float)glutGet(GLUT_ELAPSED_TIME) * 0.001f;
	float dt = (currentTime - sm_prevTime);
	sm_prevTime = currentTime;

	AudioManager::GetInstance().Process(dt);

	if (!m_pScenesStack.empty())
		m_pScenesStack.top()->Process(dt);

	InputHandler::GetInstance().Process();
}

//	OnMouseMove(x, y)
//
//	Author: Michael Jordan
//	Description:
//		Updates the position of the mouse in the inputHandler when it is moved.
//		
//	Param:
//		- int		|	x coordinate of the mouse location.
//		- int		|	y coordinate of the mouse location.
//		
//	Return: n/a		|	
//
void SceneManager::OnMouseMove(int x, int y)
{
	InputHandler::GetInstance().OnMouseMotion(x, y);

	//if (!m_pScenesStack.empty())
		//m_pScenesStack.top()->OnMousePositionChange();
}

//	OnMouseClick(button, state, x, y)
//
//	Author: Michael Jordan
//	Description:
//		Updates the mouse buttons' state in the inputHandler.
//		
//	Param:
//		- int		|	Button used on the mouse.
//		- state		|	Current status of the button.
//		- int		|	x coordinate of the mouse location.
//		- int		|	y coordinate of the mouse location.
//		
//	Return: n/a		|	
//
void SceneManager::OnMouseClick(int button, int state, int x, int y)
{
	InputHandler::GetInstance().OnMouseClick(button, state);
	InputHandler::GetInstance().OnMouseMotion(x, y);

	//if (!m_pScenesStack.empty())
		//m_pScenesStack.top()->OnMouseButtonChange();
	
}

//	OnKeyPressed(key, x, y)
//
//	Author: Michael Jordan
//	Description:
//		Updates the key status in the InputHandler.
//		
//	Param:
//		- unsigned char		|	Key pressed down.
//		- int		|	x coordinate of the mouse location.
//		- int		|	y coordinate of the mouse location.
//		
//	Return: n/a		|	
//
void SceneManager::OnKeyPressed(unsigned char key, int x, int y)
{
	InputHandler::GetInstance().OnKeyPressDown(key);
	InputHandler::GetInstance().OnMouseMotion(x, y);

	//if (!m_pScenesStack.empty())
		//m_pScenesStack.top()->OnKeyChange();
}

//	OnKeyReleased(key, x, y)
//
//	Author: Michael Jordan
//	Description:
//		Updates the key status in the InputHandler.
//		
//	Param:
//		- unsigned char		|	Key released.
//		- int		|	x coordinate of the mouse location.
//		- int		|	y coordinate of the mouse location.
//		
//	Return: n/a		|	
//
void SceneManager::OnKeyReleased(unsigned char key, int x, int y)
{
	InputHandler::GetInstance().OnKeyReleased(key);
	InputHandler::GetInstance().OnMouseMotion(x, y);

	//if (!m_pScenesStack.empty())
		//m_pScenesStack.top()->OnKeyChange();
}

//	OnSpecialPressed(key, x, y)
//
//	Author: Michael Jordan
//	Description:
//		Updates the special key status in the InputHandler.
//		
//	Param:
//		- unsigned char		|	Key released.
//		- int		|	x coordinate of the mouse location.
//		- int		|	y coordinate of the mouse location.
//		
//	Return: n/a		|	
//
void SceneManager::OnSpecialPressed(int key, int x, int y)
{
	InputHandler::GetInstance().OnSpecialPressed(key);
	InputHandler::GetInstance().OnMouseMotion(x, y);

	//if (!m_pScenesStack.empty())
		//m_pScenesStack.top()->OnSpecialChange();

}

//	OnSpecialReleased(key, x, y)
//
//	Author: Michael Jordan
//	Description:
//		Updates the special key status in the InputHandler.
//		
//	Param:
//		- unsigned char		|	Key released.
//		- int		|	x coordinate of the mouse location.
//		- int		|	y coordinate of the mouse location.
//		
//	Return: n/a		|	
//
void SceneManager::OnSpecialReleased(int key, int x, int y)
{
	InputHandler::GetInstance().OnSpecialReleased(key);
	InputHandler::GetInstance().OnMouseMotion(x, y);

	//if (!m_pScenesStack.empty())
		//m_pScenesStack.top()->OnSpecialChange();
}

//	Resize(width, height)
//
//	Author: Michael Jordan
//	Description:
//		Updates the special key status in the InputHandler.
//		
//	Param:
//		- unsigned char		|	Key released.
//		- int		|	x coordinate of the mouse location.
//		- int		|	y coordinate of the mouse location.
//		
//	Return: n/a		|	
//
void SceneManager::Resize(int width, int height)
{
	Renderer::SCREEN_WIDTH = width;
	Renderer::SCREEN_HEIGHT = height;
	Renderer::SCREEN_PROJ = glm::ortho(0.0f, (float)width, 0.0f, (float)height);
	
	glViewport(0, 0, width, height);
	
	if (!m_pScenesStack.empty())
		m_pScenesStack.top()->Resize();
}