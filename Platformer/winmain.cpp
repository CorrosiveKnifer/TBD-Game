#include "SceneManager.h"
#include "LogoScene.h"
#include "GlobalVariables.h"

int main()
{
	srand(static_cast<unsigned int>(time(0)));

	SceneManager::GetInstance().TransitionTo(new LogoScene());
	SceneManager::GetInstance().DoSceneLoop();
	
	return 0;
}