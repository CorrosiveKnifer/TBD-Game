//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	AnimMesh.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	AnimMesh
// Description:
//		Animated Mesh class which generates the VAO into the world.
//

//Self Include
#include "AnimMesh.h"

//Default Constructor
AnimMesh::AnimMesh()
{
	m_frames.x = 1;
	m_frames.y = 1;
	m_frameRate = 0;
	m_currTime = 0.0f;

	m_currFrame = glm::ivec2(0.0f, 0.0f);
	m_textureOffset = glm::vec2(0.0f, 0.0f);
	m_frameLimit = glm::vec2(0.0f, 0.0f);

	m_isLooping = true;
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Process the variable status of this mesh since the last call.
//		
//	Param:
//		- float		|	delta time since last process call.
//
//	Return: n/a		|
//
void AnimMesh::Process(float dT)
{
	if (m_frameRate != 0 && !m_isPaused) //Safety check & if paused
	{
		//Update time
		m_currTime += dT;

		//If time has elapsed
		if (m_currTime > 1.0f / m_frameRate) 
		{
			m_currTime -= 1.0f / m_frameRate;
		
			//Move to next frame.
			m_currFrame.x++;

			//If limit has been exceeded.
			if (HasXExceededLimits())
			{
				if (m_isLooping)
				{
					//Switch to the other end of the frames
					m_currFrame.x = 0;
				}
				else
				{
					//Reverse the move to the next frame and pause the anim
					m_currFrame.x = m_currFrame.x - 1;
					m_isPaused = true;
				}
			}
		}
		//Calculate the x offset
		m_textureOffset.x = m_currFrame.x * (1.0f / m_frames.x);
	}
	//Calculate the y offset
	m_textureOffset.y = m_currFrame.y * (1.0f / m_frames.y);

}

//	SetFrameSize( _frames )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Updates the frame count for this animation.
//		
//	Param:
//		- glm::uvec2	|	Inherited Matrix to create matrix upon.
//
//	Return: n/a		|
//
void AnimMesh::SetFrameSize(glm::uvec2 _frames)
{
	m_frames = _frames; 
	m_frameLimit = _frames;
	m_frameTexSize.x = 1.0f / _frames.x;
	m_frameTexSize.y = 1.0f / _frames.y;
}

//	SetCurrentFrameX( x )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Set the current x frame.
//		
//	Param:
//		- unsigned int	|	Set the current x frame of the animation.
//
//	Return: n/a		|
//
void AnimMesh::SetCurrentFrameX(unsigned int x)
{
	m_currTime = 0.0f;
	m_currFrame.x = (x < m_frames.x) ? x : m_frames.x - 1;
	m_textureOffset.x = m_currFrame.x * (1.0f / m_frames.x);
}

//	SetCurrentFrameY( y )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Set the current y frame.
//		
//	Param:
//		- unsigned int	|	Set the current y frame of the animation.
//
//	Return: n/a		|
//
void AnimMesh::SetCurrentFrameY(unsigned int y)
{
	m_currTime = 0.0f;
	m_currFrame.y = (y < m_frames.y) ? y : m_frames.y - 1;
	m_textureOffset.y = m_currFrame.y * (1.0f / m_frames.y);
}

//	HasXExceededLimits( y )
//
//	Author: Michael Jordan
//	Access: protected
//	Description:
//		Has the current frame exceeded the limitation of the animation.
//		
//	Param:
//		- n/a		|
//
//	Return: bool	|	Status of the current frame.
//
bool AnimMesh::HasXExceededLimits()
{
	return m_currFrame.x >= static_cast<int>(m_frameLimit.x) || m_currFrame.x >= static_cast<int>(m_frames.x);
}
