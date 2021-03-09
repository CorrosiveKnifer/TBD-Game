//Dependeny Includes
#include <glew.h>
#include <freeglut.h>

//Library Includes
#include <Windows.h>
#include <ctime>
#include <iostream>

//Local Includes
#include "SceneManager.h"
#include "StartScene.h"
#include "Renderer.h"
#include "InputHandler.h"
#include "AudioManager.h"
#include "LoadManager.h"
#include "LogManager.h"
#include "iniParser.h"

#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

//Close everything
void ShutDown()
{
	SceneManager::EmptyScenes();
	LogManager::DestroyInstance();
	Renderer::DestroyInstance();
	IniParser::DestroyInstance();
	InputHandler::DestroyInstance();
	AudioManager::DestroyInstance();
	LightingSystem::DestroyInstance();
	LoadManager::DestroyInstance();
}

int main(int argc, char** argv)
{
	srand(static_cast<unsigned int>(time(0))); //Set Seed

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(Renderer::SCREEN_WIDTH, Renderer::SCREEN_HEIGHT);
	
	glutCreateWindow("OpenGL Window");

	if (glewInit() != GLEW_OK)
	{
		std::cout << "ERROR 001: GLEW Initialization has failed.";
	}
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glutDisplayFunc(SceneManager::Draw);
	glutIdleFunc(SceneManager::Process);

	glutMotionFunc(SceneManager::OnMouseMove);
	glutPassiveMotionFunc(SceneManager::OnMouseMove);

	glutMouseFunc(SceneManager::OnMouseClick);

	glutKeyboardFunc(SceneManager::OnKeyPressed);
	glutKeyboardUpFunc(SceneManager::OnKeyReleased);
	glutSpecialFunc(SceneManager::OnSpecialPressed);
	glutSpecialUpFunc(SceneManager::OnSpecialReleased);
	glutReshapeFunc(SceneManager::Resize);
	glutCloseFunc(ShutDown);
	
	SceneManager::PushScene(new StartScene(), true);
	glutMainLoop();

	return (0);
}