#include "SceneManager.h"
#include "LogoScene.h"
#include "GlobalVariables.h"
#include "Renderer.h"
#include "SoundBuffer.h";
#include "iniParser.h"
#include "InputHandler.h"

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	SceneManager::GetInstance().TransitionTo(new LogoScene());
	SceneManager::GetInstance().DoSceneLoop();

	SceneManager::DestroyInstance();
	InputHandler::DestroyInstance();
	Renderer::DestroyInstance();
	SoundBuffer::DestroyInstance();
	
	return 0;
}