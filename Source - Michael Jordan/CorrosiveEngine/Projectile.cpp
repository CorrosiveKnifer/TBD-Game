//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Projectile.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Projectile
// Description:
//		Sub class of entity which travels in a single direction.
//

//Self Include
#include "Projectile.h"

//Default Constructor
Projectile::Projectile()
{
	m_pModel = 0;
	//m_moveVector = glm::vec3(1.0f, 0.0f, 0.0f);
	//m_speed = 0.0f;
	//m_width = 1.0f;

	//m_isGravity = false;

	m_isAlive = true;

	Process(0.0F);

	m_isFriendly = false;
}

//Constructor
Projectile::Projectile(std::string modelLoc, glm::vec3 _origin, glm::vec3 _direction, float _speed, unsigned int shooterIndex)
{
	m_pModel = new ModelEntity(Renderer::GetInstance().CreateModel(modelLoc.c_str()));
	this->m_localTransform.GetPositionRef() = _origin;
	this->m_localTransform.GetScaleRef() = glm::vec3(20.0f, 20.0f, 20.0f);
	m_subEntities.push_back(m_pModel);

	m_shooterIndex = shooterIndex;
	m_isAlive = true;
	Process(0.0F);
}

//Destructor
Projectile::~Projectile()
{
	//Not incharge of models
}

//	Draw( program )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Draws this proj to the scene.
//		
//	Param:
//		- GLuint	|	Main rendering program ID.	
//
//	Return: n/a		|
//
void Projectile::Draw(Camera* camera, Program* program)
{
	Entity::Draw(camera, program);
}

//	Process( inheritedMatrix, dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes this proj since the last call.
//		
//	Param:
//		- glm::mat4	|	Inherited Matrix
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void Projectile::Process(float dT)
{
	//m_transform.GetRotationRef() = glm::angleAxis(glm::radians(-90.0f) + atan2(m_moveVector.x, m_moveVector.z), glm::vec3(0.0f, 1.0f, 0.0f));

	if (m_isAlive)
	{
		m_lifeTime += dT;
		m_isAlive = (m_lifeTime < 2.0f);
	}

	Entity::Process(dT);
}

//	SetSize( _width, _height )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Updates the size and scale of the projectile
//		
//	Param:
//		- float		|	width of the proj.
//		- float		|	height of the proj
//
//	Return: n/a		|
//
void Projectile::SetSize(float _width, float _height)
{
	//m_width = _width;
	//m_height = _height;

	//float xScale = (m_width * 0.5f) / Renderer::SCREEN_WIDTH;
	//float yScale = (m_height * 0.5f) / Renderer::SCREEN_HEIGHT;

	//if (m_pModel != 0)
		//SetLocalScale(glm::vec3(xScale, yScale, 1.0f));
}

//	SetDirection( _direction )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Updates the direction of the proj
//		
//	Param:
//		- glm::vec3		|	Directional vector.
//
//	Return: n/a		|
//
void Projectile::SetDirection(glm::vec3 _direction)
{
	//if (_direction != m_moveVector)
	//{
	//	m_moveVector = glm::normalize(_direction);
	//}
}

//	SetSpeed( _speed )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Updates the speed of the proj
//		
//	Param:
//		- float		|	Directional vector.
//
//	Return: n/a		|
//
void Projectile::SetSpeed(float _speed)
{
	//if (_speed != m_speed)
	//{
	//	m_speed = _speed;
	//}
}
