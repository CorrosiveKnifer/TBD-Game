#include "DebugManager.h"

#include <windows.h>
#include <Thread>
#include <iostream>

DebugManager* DebugManager::sm_pInstance = nullptr;

DebugManager& DebugManager::GetInstance()
{
	if (sm_pInstance == nullptr)
	{
		sm_pInstance = new DebugManager();
	}
	return *sm_pInstance;
}

void DebugManager::DestroyInstance()
{
	if (sm_pInstance != nullptr)
	{
		delete sm_pInstance;
	}
	sm_pInstance = nullptr;
}

void DebugManager::StartDebug()
{
	std::map<std::string, Cheat>* _cheats = &m_cheats;
	HWND _console = GetConsoleWindow();

	std::thread t([_cheats, _console]()
		{
			SetFocus(_console);

			std::cout << "DEBUG MODE: ";
			
			std::string tempString;
			std::cin >> tempString;

			if (_cheats->find(tempString) != _cheats->end())
			{
				Cheat* cheat = &_cheats->find(tempString)->second;
				cheat->state = !cheat->state;

				std::cout << cheat->descipt << ":";

				if (cheat->state)
					std::cout << "Enabled";
				else
					std::cout << "Disabled";

				std::cout << std::endl;
			}
			else
			{
				std::cout << "cheat not found" << std::endl;
			}
		});
	t.detach();
}
