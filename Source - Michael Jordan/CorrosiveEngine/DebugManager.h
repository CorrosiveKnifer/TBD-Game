#ifndef _DEBUG_MANAGER_H_
#define _DEBUG_MANAGER_H_

#include <map>
#include <string>

struct Cheat
{
	Cheat()
		: descipt("null"), state(false) {}

	Cheat(std::string _descipt)
		: descipt(_descipt) , state(false){}

	std::string descipt;
	
	bool state;
};

class DebugManager
{
public:
	static DebugManager& GetInstance();
	static void DestroyInstance();

	void StartDebug();
	bool GetCheatState(std::string _cheatCommand) { return m_cheats[_cheatCommand].state; };
protected:

private:
	DebugManager() 
	{
		m_cheats.insert(std::pair<std::string, Cheat>("TFC", Cheat("Toggle Free Camera")));
		m_cheats.insert(std::pair<std::string, Cheat>("ShowColl", Cheat("Show Colliders")));
	};

	~DebugManager() {};

public:

protected:
	std::map<std::string, Cheat> m_cheats;
private:
	static DebugManager* sm_pInstance;

};

#endif //_DEBUG_MANAGER_H_