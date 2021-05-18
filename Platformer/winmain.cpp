#include "SceneManager.h"
#include "LogoScene.h"
#include "GlobalVariables.h"

int main()
{
	SceneManager::GetInstance().TransitionTo(new LogoScene());
	SceneManager::GetInstance().DoSceneLoop();
	
	return 0;
}