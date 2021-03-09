#ifndef _POSTAL_SERVICE_H_
#define _POSTAL_SERVICE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Renderer.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Renderer
// Description:
//		A class inwhich sorts the packet information into priorities for the server.
//

//Local Includes
#include "GlobalStructures.h"

//Library Includes
#include <queue>

class PostalService
{
	//Member Functions:
public:
	PostalService() {};
	~PostalService() {};

	void QueueImportantPacket(Global::NetworkPacket _packet) { m_importantPackets.push(_packet); };
	void QueueExpressPacket(Global::NetworkPacket _packet) { m_expressPackets.push(_packet); };
	void QueueNormalPacket(Global::NetworkPacket _packet) { m_normalPackets.push(_packet); };

	bool DequePackage(char* _package);

	bool Empty(){
		return m_importantPackets.empty() && m_expressPackets.empty() && m_normalPackets.empty();
	};
protected:
	
private:
	
	//Member Data:
public:
	std::queue<Global::NetworkPacket> m_importantPackets;
	std::queue<Global::NetworkPacket> m_expressPackets;
	std::queue<Global::NetworkPacket> m_normalPackets;
protected:

private:
};

#endif // !_GLOBAL_STRUCT_H_