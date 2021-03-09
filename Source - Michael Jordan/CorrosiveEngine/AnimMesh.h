#ifndef _ANIM_MESH_H_
#define _ANIM_MESH_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	AnimMesh.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	AnimMesh
// Description:
//		Animated Mesh class which generates the VAO into the world.
//

//Parent Include
#include "Mesh.h"

class AnimMesh :public Mesh
{
	//Member Functions:
public:
	
	AnimMesh();
	virtual ~AnimMesh() {};

	void Process(float dT);
	
	void ResetFrameX() { m_currFrame.x = 0; };
	void ResetFrameY() { m_currFrame.y = 0; };
	void SetFrameSize(glm::uvec2 _frames);
	void SetFrameRate(int rate) { m_frameRate = rate; };
	void SetCurrentFrameX(unsigned int x);
	void SetCurrentFrameY(unsigned int y);
	void SetFrameLimit(glm::uvec2 _limit) { m_frameLimit = _limit; };
	glm::vec2 GetCurrentFrame() { return m_currFrame; };

	void SetLooping(bool _loop) { m_isLooping = _loop; };
	void SetPause(bool _pause) { m_isPaused = _pause; };

	bool IsPause() { return m_isPaused; };

protected:
	bool HasXExceededLimits();

private:

	//Member Data:
public:

protected:
	glm::uvec2 m_frames;
	glm::ivec2 m_currFrame;
	glm::uvec2 m_frameLimit;

	int m_frameRate;
	float m_currTime;

	bool m_isPaused;
	bool m_isLooping;
private:

};

#endif // !_ANIM_MESH_H_