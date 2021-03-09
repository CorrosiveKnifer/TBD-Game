#ifndef _LOGMANAGER_H_
#define _LOGMANAGER_H_

class LogManager
{
	//Member Functions:
public:
	static LogManager& GetInstance();
	static void DestroyInstance();

	void Log(const char* _msg);
	void LogErr(const char* _msg);

protected:

private:
	LogManager() {};
	~LogManager() {};

	//Member Data:
public:

protected:
	static LogManager* sm_pInstance;
private:

};

#endif // !_LOGMANAGER_H_