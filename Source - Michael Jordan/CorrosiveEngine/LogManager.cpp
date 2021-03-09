#include "LogManager.h"

#include <iostream>
#include <windows.h>

LogManager* LogManager::sm_pInstance = nullptr;

LogManager& LogManager::GetInstance()
{
	if (sm_pInstance == nullptr)
	{
		sm_pInstance = new LogManager();
	}

	return *sm_pInstance;
}

void LogManager::DestroyInstance()
{
	if (sm_pInstance != nullptr)
	{
		delete sm_pInstance;
	}
}

void LogManager::Log(const char* _msg)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
	std::cout << _msg << std::endl;
}

void LogManager::LogErr(const char* _msg)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 12);
	std::cout << _msg << std::endl;
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}
