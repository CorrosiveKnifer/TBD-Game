#ifndef _PLAYER_TWO_D_H_
#define _PLAYER_TWO_D_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Player.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Player
// Description:
//		Main player class to navigate the world with.
//

//Parent Include
#include "Entity.h"

//Local Include
#include "MeshEntity.h"
#include "Camera.h"
#include "ParticleSystem.h"
#include "Explosion.h"
#include "LightingSystem.h"
#include "ThreeDGameScene.h"

class PlayerTwo : public Entity
{
	//Member Functions:
public:
	PlayerTwo(Camera* cam);
	virtual ~PlayerTwo();

	virtual void Draw(Camera* camera, Program* program); //To render object to the screen.
	virtual void Process(float dT); //To update the object.

	void ProcessInput(float dt);
	void ProcessDestruction(float dT);

	void Move(int direct) { if (!m_isDestroyed) m_moveDirect = direct; };
	void Rotate(int direct) { if (!m_isDestroyed) m_rotatDirect = direct; };
	void RotateCannon(glm::vec2 unitVect) { m_targetDirect = unitVect; };
	

	void StartFalling() { m_isFalling = true; };

	void SetHealth(float health) { m_health = health; };
	float GetHealth() { return m_health; };
	bool IsDead() { return m_isDestroyed /*&& m_explosion.IsFinished()*/; };
	bool IsDestroyed() { return m_isDestroyed; };
	void SetColour(glm::vec3 colour) { m_colour = colour; };

protected:
	void Fire();

private:
	//Member Data:
public:

protected:
	Entity* m_pGunMotor;
	MeshEntity* m_pBody;
	//ModelEntity* m_pGun;

	Model* m_pProjModel;

	glm::vec2 m_gunTarget;
	glm::vec3 m_gunOffset;

	glm::vec3 m_colour;
	float m_gunRotOffset;
	Transform m_gunMat;

	float m_GunDelay;
	float m_health;

	Camera* m_camera;
	float m_playerRotation;
	float m_playerRotationStep;
	float m_turretRotationSpeed;
	int m_moveDirect;
	int m_rotatDirect;

private:
	bool m_canFire;
	bool m_isDestroyed;
	bool m_isFalling;

	glm::vec2 m_currentDirect;
	glm::vec2 m_targetDirect;
};

#endif // !_PLAYER_TWO_D_H_