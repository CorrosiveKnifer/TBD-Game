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

//Self Includes
#include "PostalService.h"

//Library Includes
#include <string>

//	DequePackage( _package )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Package packed with packets in order of priority.
//		
//	Param:
//		- char*	|	string of maximum size 200.
//
//	Return: bool	|	if package was successfully packed.
//
bool PostalService::DequePackage(char* _package)
{
	Global::NetworkPacket packet;
	std::string message;
	unsigned int index;

	while (!m_importantPackets.empty())
	{ //Deque important messages until maximum is reached.
		int newSize = m_importantPackets.front().PacketSize + message.size() + 2;

		if (newSize > MAX_MESSAGE_SIZE)
		{
			//Package is big enough
			strcpy_s(_package, MAX_MESSAGE_SIZE, message.c_str());
			return true;
		}
		else
		{
			//Add package
			message += "<";
			message += m_importantPackets.front().packetData;
			message += ">";
			m_importantPackets.pop();
		}

	}

	while (!m_expressPackets.empty())
	{ //Deque important messages until maximum is reached.
		int newSize = m_expressPackets.front().PacketSize + message.size() + 2;

		if (newSize > MAX_MESSAGE_SIZE)
		{
			//Package is big enough
			strcpy_s(_package, MAX_MESSAGE_SIZE, message.c_str());
			return true;
		}
		else
		{
			//Add package
			message += "<";
			message += m_expressPackets.front().packetData;
			message += ">";
			m_expressPackets.pop();
		}
	}

	while (!m_normalPackets.empty()) 
	{ //Deque normal messages until maximum is reached.
		int newSize = m_normalPackets.front().PacketSize + message.size() + 2;

		if (newSize > MAX_MESSAGE_SIZE)
		{
			//Package is big enough
			strcpy_s(_package, MAX_MESSAGE_SIZE, message.c_str());
			return true;
		}
		else
		{
			//Add package
			message += "<";
			message += m_normalPackets.front().packetData;
			message += ">";
			m_normalPackets.pop();
		}
	}

	if (message.size() > 0)
	{
		strcpy_s(_package, MAX_MESSAGE_SIZE, message.c_str());
		return true;
	}

	return false;
}
