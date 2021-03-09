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

//Self Include
#include "PlayerTwo.h"

//Local Include
#include "SceneManager.h"
#include "InputHandler.h"
#include "AudioManager.h"
#include "BoxCollider.h"
#include "ModelEntity.h"
#include "DebugManager.h"
#include "Projectile.h"

//Constructor
PlayerTwo::PlayerTwo(Camera* cam)
{
	//m_collider = new BoxCollider();
	//m_collider->SetScale(glm::vec3(2.35f, 1.5f, 3.5f));

	//m_isGravity = true;
	m_camera = cam;

	//Body Mesh
	//m_subEntities.push_back()
	m_pBody = new MeshEntity(Renderer::GetInstance().CreateSquareMesh());
	m_pBody->SetTexture(Renderer::GetInstance().CreateTexture("PlayerTemp.png"));
	m_pBody->GetTransformRef().GetScaleRef() = glm::vec3(100.0f, 150.0f, 1.0f);
	m_subEntities.push_back(m_pBody);

	m_currentDirect = glm::vec2(0.0f, 0.0f);
	m_health = 100.0f;
	//m_isGrounded = true;
	m_canFire = true;
	m_isDestroyed = false;
	m_isFalling = false;
	m_colour = glm::vec3(0.0f, 1.0f, 0.0f);

	//Default vectors
	//m_moveVector = glm::vec3(0.0f, 0.0f, 1.0f);

	//Arm Mesh
	m_gunTarget = glm::vec2(0.0f, 0.0f);
	m_gunRotOffset = 0.0f;

	//m_maxSpeed = 30.0f;
	m_moveDirect = 0;
	m_rotatDirect = 0;
	m_turretRotationSpeed = 0.18f;
	m_playerRotationStep = 50.0f;
	m_playerRotation = 0.0f;
	m_GunDelay = 0.0f;

	//LightingSystem::GetInstance().AddSpotLight(m_frontLight);
	//LightingSystem::GetInstance().AddSpotLight(m_backLight);

	//Preload Audio
	AudioManager::GetInstance().CreateSound("Fire_SE_0.wav", FMOD_DEFAULT);
	AudioManager::GetInstance().CreateSound("Fire_SE_1.wav", FMOD_DEFAULT);
	AudioManager::GetInstance().CreateSound("Fire_SE_2.wav", FMOD_DEFAULT);
	AudioManager::GetInstance().CreateSound("Reload_SE.wav", FMOD_DEFAULT);
	AudioManager::GetInstance().CreateSound("Treads_0.wav", FMOD_LOOP_NORMAL);
}

//Destructor
PlayerTwo::~PlayerTwo()
{
	//Not incharge of models
	m_pBody = 0;
	m_pProjModel = 0;
	//m_pGun = 0;

	//TODO: Before multiplayer remove specific spot lights
	//at the moment there is no need to, and are removed once gamescene is deleted.
	//m_frontLight = 0;
	//m_backLight = 0;

	m_camera = 0;
}

//	Draw( program )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Draws this player's to the scene.
//		
//	Param:
//		- GLuint	|	Main rendering program ID.	
//
//	Return: n/a		|
//
void PlayerTwo::Draw(Camera* camera, Program* program)
{
	Entity::Draw(camera, program);
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes this player since the last call.
//		
//	Param:
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void PlayerTwo::Process(float dT)
{
	if(m_camera != nullptr && !m_camera->IsInTransition())
		ProcessInput(dT);

	////Delay inbetween shots
	if (m_GunDelay > 0)
		m_GunDelay -= dT;

	//Process
	Entity::Process(dT);
}

void PlayerTwo::ProcessInput(float dt)
{
	if (DebugManager::GetInstance().GetCheatState("TFC"))
		return;

	int vertAxis = 0;
	int horizAxis = 0;

	if (InputHandler::GetInstance().IsKeyPressed('w') || InputHandler::GetInstance().IsKeyPressed('W'))
	{
		vertAxis++;
	}
	if (InputHandler::GetInstance().IsKeyPressed('a') || InputHandler::GetInstance().IsKeyPressed('A'))
	{
		horizAxis--;
	}
	if (InputHandler::GetInstance().IsKeyPressed('s') || InputHandler::GetInstance().IsKeyPressed('S'))
	{
		vertAxis--;
	}
	if (InputHandler::GetInstance().IsKeyPressed('d') || InputHandler::GetInstance().IsKeyPressed('D'))
	{
		horizAxis++;
	}
	if (InputHandler::GetInstance().IsMousePressedFirst(GLUT_LEFT_BUTTON))
	{
		Fire();
	}

	glm::vec3 direct = Renderer::GetInstance().GetMouseDirectionToWorld(m_camera);
	float dist = -10;//-m_camera->GetWorldPosition().y;
	glm::vec3 mouse = Renderer::GetInstance().GetPositonFromRayOnPlane(m_camera->GetWorldPosition(), direct, glm::vec3(0.0f, 1.0f, 0.0f), dist);

	m_gunTarget = glm::vec2(m_localTransform.GetPosition().x - mouse.x, m_localTransform.GetPosition().z - mouse.z);
	m_gunRotOffset = glm::radians(180.0f) + atan2(m_gunTarget.x, m_gunTarget.y);
	//m_pGunMotor->GetTransformRef().GetRotationRef() = glm::angleAxis(m_gunRotOffset, glm::vec3(0.0f, 1.0f, 0.0f));

	float speed = 5.0f;
	m_localTransform.GetPositionRef() += m_localTransform.GetRight() * (vertAxis * speed * dt);
	m_localTransform.GetPositionRef() += m_localTransform.GetUp() * (horizAxis * speed * dt);

	m_localTransform.GetPositionRef() = glm::vec3(m_localTransform.GetPosition().x, m_localTransform.GetPosition().y, 0.0f);
}

//	ProcessDestruction( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes only the destruction animation of the player.
//		
//	Param:
//		- float		|	Change in time since last delta time.
//
//	Return: n/a		|
//
void PlayerTwo::ProcessDestruction(float dT)
{
	//Process explosion
	if (m_isDestroyed)
	{
		//m_explosion.SetLocalPosition(m_transform.GetPosition());
		//m_explosion.Process(dT);
	}
}

//	Fire( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Force transition this player into the shooting state.
//		
//	Param:
//		- GameScene*	|	Scene to render bullet too.
//
//	Return: n/a		|
//
void PlayerTwo::Fire()
{
	//if (!m_isDestroyed && m_GunDelay <= 0)
	//{
	//	glm::vec3 direct = glm::vec3(0.0f, 0.0f, 0.0f);
	//	direct.x = sin(glm::radians(m_gunRotOffset));
	//	direct.z = cos(glm::radians(m_gunRotOffset));
	//
	//	ModelEntity* temp = new ModelEntity(Renderer::GetInstance().CreateModel("Bullet/Bullet.obj"));
	//	temp->GetTransformRef().GetPositionRef() = m_localTransform.GetPosition();
	//	temp->GetTransformRef().GetScaleRef() = glm::vec3(10.0f, 10.0f, 10.0f);
	//	//new Projectile("Bullet/Bullet.obj", m_pGun->GetTransform().GetPosition(), direct, 25.0f, 0)
	//	SceneManager::InstantiateEntity(temp);
	//
	//	//scene->SpawnBullet(
	//	//	m_pProjModel,
	//	//	m_localTransform.GetPosition() + glm::vec3(m_gunTarget.x, 0, m_gunTarget.y) + 3.0f * glm::vec3(sin(glm::radians(m_gunRotOffset)), 0.0f, cos(glm::radians(m_gunRotOffset))),
	//	//	direct,
	//	//	25.0f
	//	//);
	//
	//	m_GunDelay = glm::radians(45.0f);
	//	m_canFire = false;
	//	AudioManager::GetInstance().PlaySoundEffect("Fire_SE_" + std::to_string(rand() % 3) + ".wav");
	//	m_camera->Shake(0.8f, glm::vec3(1.0f, 0.0f, 1.0f));
	//}
}