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
#include "Player.h"

//Local Include
#include "SceneManager.h"
#include "InputHandler.h"
#include "AudioManager.h"
#include "BoxCollider.h"
#include "ModelEntity.h"
#include "DebugManager.h"
#include "Projectile.h"

//Constructor
Player::Player(Camera* cam)
{
	//m_collider = new BoxCollider();
	//m_collider->SetScale(glm::vec3(2.35f, 1.5f, 3.5f));

	//m_isGravity = true;
	m_camera = cam;

	//Body Mesh
	//m_subEntities.push_back()
	m_pBody = new ModelEntity(Renderer::GetInstance().CreateModel("TankBase/TankBase.obj"));
	m_pBody->GetTransformRef().GetScaleRef() = glm::vec3(0.2f, 0.2f, 0.2f);
	m_pBody->SetColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	m_subEntities.push_back(m_pBody);

	m_pGunMotor = new Entity();
	//m_pBody->AddChild(m_pGunMotor);
	m_subEntities.push_back(m_pGunMotor);

	m_pGun = new ModelEntity(Renderer::GetInstance().CreateModel("TankGun/TankGun.obj"));
	m_gunOffset = glm::vec3(0.0f, 1.0f, 0.0f);
	m_pGun->GetTransformRef().GetPositionRef() = m_gunOffset;
	m_pGun->GetTransformRef().GetScaleRef() = glm::vec3(0.2f, 0.2f, 0.2f);
	m_pGun->SetColour(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));
	m_pGunMotor->AddChild(m_pGun);

	//m_pProjModel = &Renderer::GetInstance().CreateModel("Bullet/Bullet.obj");

	//Default values
	//m_width = 3.0f;
	//m_height = 3.0f;

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

	//Lights
	m_frontLight = LightingSystem::GetInstance().CreateSpotLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.1f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	m_backLight = LightingSystem::GetInstance().CreateSpotLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -0.2f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f), 7.5f, 15.0f);

	//For distance of 100f
	m_frontLight->constant = 1.0f;
	m_frontLight->linear = 0.045f;
	m_frontLight->quadratic = 0.0075f;

	//For distance of 150f
	m_backLight->constant = 1.0f;
	m_backLight->linear = 0.027f;
	m_backLight->quadratic = 0.0028f;

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
Player::~Player()
{
	//Not incharge of models
	m_pBody = 0;
	m_pProjModel = 0;
	m_pGun = 0;

	//TODO: Before multiplayer remove specific spot lights
	//at the moment there is no need to, and are removed once gamescene is deleted.
	m_frontLight = 0;
	m_backLight = 0;

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
void Player::Draw(Camera* camera, Program* program)
{
	glm::mat4 temp = m_localTransform.GetModelMatrix();
	//m_pBody->Draw(camera, program);

	//m_pGun->SetColour(glm::vec4(m_colour * 0.85f, 1.0f));
	//m_pGun->SetInheritedMatrix(m_gunMat.GetModelMatrix());
	//m_pGun->Draw(camera, program);
	
	if (m_isDestroyed)
	{
		//m_explosion.Draw(camera);
	}

	//Renderer::GetInstance().DrawPoint(camera, mouse);

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
void Player::Process(float dT)
{
	if(m_camera != nullptr && !m_camera->IsInTransition())
		ProcessInput(dT);

	////Delay inbetween shots
	if (m_GunDelay > 0)
		m_GunDelay -= dT;

	//
	////Play reload sound:
	//if (!m_canFire && m_GunDelay <= 0.0f)
	//{
	//	m_canFire = true;
	//	AudioManager::GetInstance().PlaySoundEffect("Reload_SE.wav");
	//}
	//
	////Using LERP to transition to new direction
	//m_currentDirect.x += (m_targetDirect.x - m_currentDirect.x) * m_turretRotationSpeed;
	//m_currentDirect.y += (m_targetDirect.y - m_currentDirect.y) * m_turretRotationSpeed;
	//
	////Update rotation information
	//m_gunRotOffset = glm::radians(180.0f) + atan2(m_currentDirect.x, m_currentDirect.y);
	//
	////If not grounded apply gravity
	//if (m_isFalling)
	//{
	//	//m_transform.GetPositionRef().y -= 0.5f;
	//}
	//
	////Forward/Backwards Motion
	//if (m_moveDirect == 0)
	//{
	//	//m_accel = 0;
	//	//m_speed += (0 - m_speed) * 0.15f;
	//}
	//else
	//{
	//	//m_accel = m_moveDirect * 5.0f;
	//}

	//Rotation Motion
	//if (abs(m_speed) > 0.65f)
	//{
	//	m_playerRotation += m_rotatDirect * m_playerRotationStep * dT;
	//
	//	if (m_rotatDirect != 0)
	//	{
	//		m_transform.GetRotationRef() = glm::angleAxis(glm::radians(m_playerRotation), glm::vec3(0.0f, 1.0f, 0.0f));
	//		
	//		m_moveVector = m_transform.GetForward();
	//	}
	//}

	//Toggle Treads
	//PlayTreads();
	m_pGun->GetTransformRef().GetPositionRef() = m_gunOffset + m_pGun->GetTransformRef().GetForward() * -sin(m_GunDelay);

	//Process
	Entity::Process(dT);

	//Point Gun towards Mouse
	//glm::vec3 LookDirect = glm::vec3(sin(m_gunTarget.x), 0.0f, cos(m_gunTarget.y));
	
	//m_gunMat.GetRotationRef() = glm::angleAxis(m_gunRotOffset, glm::vec3(0.0f, 1.0f, 0.0f));
	//
	////Update lighting
	////m_frontLight->position = m_transform.GetPosition() + 2.0f * glm::vec3(sin(glm::radians(m_playerRotation)), 0.0f, cos(glm::radians(m_playerRotation)));
	//m_frontLight->direction = glm::vec3(sin(glm::radians(m_playerRotation)), -0.1f, cos(glm::radians(m_playerRotation)));
	//
	//m_backLight->position = m_localTransform.GetPosition() + m_gunPosOffset + 1.0f * glm::vec3(sin(glm::radians(m_gunRotOffset)), 0.0f, cos(glm::radians(m_gunRotOffset)));
	//m_backLight->direction = glm::vec3(sin(glm::radians(m_gunRotOffset)), -0.2f, cos(glm::radians(m_gunRotOffset)));
}

void Player::ProcessInput(float dt)
{
	if (DebugManager::GetInstance().GetCheatState("TFC"))
		return;

	int vertAxis = 0;
	int horizAxis = 0;

	if (InputHandler::GetInstance().IsKeyPressed('w') || InputHandler::GetInstance().IsKeyPressed('W'))
	{
		vertAxis--;
	}
	if (InputHandler::GetInstance().IsKeyPressed('a') || InputHandler::GetInstance().IsKeyPressed('A'))
	{
		horizAxis--;
	}
	if (InputHandler::GetInstance().IsKeyPressed('s') || InputHandler::GetInstance().IsKeyPressed('S'))
	{
		vertAxis++;
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
	m_pGunMotor->GetTransformRef().GetRotationRef() = glm::angleAxis(m_gunRotOffset, glm::vec3(0.0f, 1.0f, 0.0f));

	float speed = 5.0f;
	m_localTransform.GetPositionRef() += m_localTransform.GetForward() * (vertAxis * speed * dt);
	m_localTransform.GetPositionRef() += m_localTransform.GetUp() * (horizAxis * speed * dt);
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
void Player::ProcessDestruction(float dT)
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
void Player::Fire()
{
	if (!m_isDestroyed && m_GunDelay <= 0)
	{
		glm::vec3 direct = glm::vec3(0.0f, 0.0f, 0.0f);
		direct.x = sin(glm::radians(m_gunRotOffset));
		direct.z = cos(glm::radians(m_gunRotOffset));

		ModelEntity* temp = new ModelEntity(Renderer::GetInstance().CreateModel("Bullet/Bullet.obj"));
		temp->GetTransformRef().GetPositionRef() = m_localTransform.GetPosition();
		temp->GetTransformRef().GetScaleRef() = glm::vec3(10.0f, 10.0f, 10.0f);
		//new Projectile("Bullet/Bullet.obj", m_pGun->GetTransform().GetPosition(), direct, 25.0f, 0)
		SceneManager::InstantiateEntity(temp);

		//scene->SpawnBullet(
		//	m_pProjModel,
		//	m_localTransform.GetPosition() + glm::vec3(m_gunTarget.x, 0, m_gunTarget.y) + 3.0f * glm::vec3(sin(glm::radians(m_gunRotOffset)), 0.0f, cos(glm::radians(m_gunRotOffset))),
		//	direct,
		//	25.0f
		//);

		m_GunDelay = glm::radians(45.0f);
		m_canFire = false;
		AudioManager::GetInstance().PlaySoundEffect("Fire_SE_" + std::to_string(rand() % 3) + ".wav");
		m_camera->Shake(0.8f, glm::vec3(1.0f, 0.0f, 1.0f));
	}
}

//	GetBounds( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Converts position information and size  of this enemy into usable 
//		bounds for collision.
//		
//	Param:
//		-	n/a
//
//	Return: Bound	|	Bounds of this entity
//	
Bound Player::GetBounds()
{
	//Hit box when not crouching
	float left = 0; //m_transform.GetPositionRef().x - (m_width * 0.5f);
	float top = 0;// m_transform.GetPositionRef().y - (-m_height * 0.5f);

	return Bound(left, top, 0, -0);
}

//	HandleDamage( damage )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Damage the player.
//		
//	Param:
//		-	float	|	Damage dealt to the player.
//
//	Return: n/a		|
//	
void Player::HandleDamage(float damage)
{
	m_health -= damage;
	if (m_health <= 0 && !m_isDestroyed)
	{
		StartDestroy();
	}
}

//	StartDestroy( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Sets up the particle system to start emiting and
//		disables all control from the player.
//		
//	Param:
//		-	n/a		|
//
//	Return: n/a		|
//	
void Player::StartDestroy()
{
	//m_explosion.SetLocalPosition(m_transform.GetPosition());
	//m_explosion.SetSpawnTimer(0.1f);
	//m_explosion.SetLimit(25);
	//m_explosion.SetLocalScale(glm::vec3(0.75f, 0.75f, 0.75f));
	m_isDestroyed = true;
	//m_explosion.Start();
}

//	Respawn( pos )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Resets the player and teleports the player to the specified
//		location.
//		
//	Param:
//		-	glm::vec3	|	positon to respawn the player too.
//
//	Return: n/a		|
//	
void Player::Respawn(glm::vec3 pos)
{
	m_isDestroyed = false;
	m_isFalling = false;
	m_health = 100.0f;
	m_localTransform.GetPositionRef() = pos;
	m_localTransform.GetRotationRef() = glm::quat();
	m_moveDirect = 0;
	//m_explosion.Restart();
	//m_moveVector = glm::vec3(0.0f, 0.0f, 1.0f);
	m_playerRotation = 0.0f;
	//m_speed = 0;
	//m_accel = 0;
}

//	PlayTreads( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Toggle treads sound effect when moving.
//		
//	Param:
//		-	n/a		|
//
//	Return: n/a		|
//	
void Player::PlayTreads()
{
	//if ((int)(m_speed + 0.4f) > 0 && m_moveDirect != 0)
	//{
	//	AudioManager::GetInstance().PlaySoundEffectOnce("Treads_0.wav");
	//}
	//else
	//{
	//	AudioManager::GetInstance().StopOneSoundEffect("Treads_0.wav");
	//}
}
