//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Entity.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Entity
// Description:
//		Main object within the scene.
//

//Self include
#include "Entity.h"
#include "Collider.h"
#include "DebugManager.h"

//Constructor
Entity::Entity()
{
	
}

//Destructor
Entity::~Entity()
{
	
}

void Entity::Draw(Camera* camera, Program* program)
{
	Transform _worldTransform = Transform::ExtractTransform(m_inheritMatrix * m_localTransform.GetModelMatrix());

	for (Component* component : m_components)
	{
		component->Draw(camera, program, _worldTransform);
	}

	//Draw all sub entities
	for (Entity* entity : m_subEntities)
	{
		entity->Draw(camera, program);
	};

	if (DebugManager::GetInstance().GetCheatState("ShowColl"))
	{
		//m_collider->Draw(camera);
	}
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes this entity since the last call.
//		
//	Param:
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void Entity::Process(float dT)
{
	Transform _worldTransform = Transform::ExtractTransform(m_inheritMatrix * m_localTransform.GetModelMatrix());

	//Process all components
	for (Component* component : m_components)
	{
		component->Process(dT);
	}

	//Process all subentities
	for (Entity* entity : m_subEntities)
	{
		entity->SetInheritMatrix(_worldTransform.GetModelMatrix());
		entity->Process(dT);
	};
}

//	Deserialize( update )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Deconstructs a string only to get the information relevant for
//		this entity. Getting the position and rotation information of this entity
//		before passing it on to the subclass to deal with more information.
//		
//	Param:
//		- std::string	|	string to deconstruct.
//
//	Return: std::string		|	string that has been deconstructed for the subclasses.
//
std::string Entity::Deserialize(std::string update)
{
	//<x>,<y>,<z>,<ry>,
	//update = update.substr(update.find_first_of(' ') + 1);
	//
	//std::string val = update.substr(0, 5);
	//m_position.x = std::stof(val);
	//update = update.substr(6);
	//
	//val = update.substr(0, 5);
	//m_position.y = std::stof(val);
	//update = update.substr(6);
	//
	//val = update.substr(0, 5);
	//m_position.z = std::stof(val);
	//update = update.substr(6);
	//
	//val = update.substr(0, 5);
	//m_rotation.y = std::stof(val);
	//update = update.substr(6);
	//
	//UpdateModelMatrix();

	return update;
}

bool Entity::AddChild(Entity* _child)
{
	for (Entity* e : m_subEntities)
	{
		if (&(*e) == &(*_child))
			return false;
	}
	m_subEntities.push_back(_child);
	return true;
}

bool Entity::AddComponent(Component* _component)
{
	for (Component* c : m_components)
	{
		if (&(*c) == &(*_component))
			return false;
	}
	m_components.push_back(_component);
	return true;
}

//	IsBoxCollidingWith( other )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Checks whether this entity is overlapping with another enitity
//		using the box vs. box collision.
//		
//	Param:
//		-	Entity&		|	other entity to check.
//
//	Return: bool	|	Status of the overlapp in both x-axis and y-axis.
//
//bool Entity::IsBoxCollidingWith(Entity& other) const
//{
//	if (!m_canCollide || !other.GetCollide())
//		return false;
//
//	//Get this entities' headBounds
//	//Bound myBounds = this->GetBounds();
//
//	//Get the other entities' feetBounds
//	//Bound otherBounds = other.GetBounds();
//
//	//Check if the x-Axis overlaps
//	//bool xAxis = myBounds.m_left < otherBounds.GetRight() && myBounds.GetRight() > otherBounds.m_left;
//
//	//Check if the y-Axis overlaps
//	//bool yAxis = myBounds.m_top > otherBounds.GetBottem() && myBounds.GetBottem() < otherBounds.m_top;
//
//	//return xAxis && yAxis;
//
//	return false;
//}


//	IsCircleCollidingWith( other )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Checks whether this entity is overlapping with another enitity
//		using the circle vs. circle collision.
//		
//	Param:
//		-	Entity&		|	other entity to check.
//
//	Return: bool	|	Status of the overlapp in both x-axis and y-axis.
//
//bool Entity::IsCircleCollidingWith(Entity& other) const
//{
//	;
//	float dist = glm::distance(this->m_transform.GetPosition(), other.GetTransform().GetPosition());
//
//	float radius = this->m_width + other.GetWidth();
//
//	return dist <= radius;
//}

//	GetBounds( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Converts position information and size  of this entity into usable 
//		bounds for collision.
//		
//	Param:
//		-	n/a
//
//	Return: Bound	|	converted coordinates.
//
//Bound Entity::GetBounds() const
//{
//	//float left = m_position.x - (m_width * 0.5f);
//	//float top = m_position.y - (-m_height * 0.5f);
//
//	//return Bound(left, top, m_width, -m_height);
//}

//	GetRotatedCoord( glm::vec2)
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Rotates coordinates around the origin by this entities' rotation on the z-axis.
//		
//	Param:
//		-	glm::vec2	|	orignial coord
//
//	Return: glm::vec2	|	rotated coordinate.
//
//glm::vec2 Entity::GetRotatedCoord(glm::vec2 orignal)
//{
//	glm::mat4 currMat = glm::translate(glm::mat4(), m_position);
//	currMat = glm::rotate(currMat, glm::radians(m_rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));
//	currMat = glm::translate(currMat, -m_position);
//	
//	glm::vec4 temp = currMat * glm::vec4(orignal, 0.0f, 1.0f);
//
//	return glm::vec2(temp.x, temp.y);
//}

//	HandleCollision( other) [2D]
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Calculates the total overlap of this entity on another entity,
//		then pushes this away to remove any collision. Also sets grounding
//		for this entity if and only if the north of other was overlapped.
//		
//	Param:
//		-	Entity&	|	other, unmoveable entity.
//
//	Return: n/a		|
//
//void Entity::HandleCollision(Entity& other)
//{
//	if (m_rotation.z != other.GetLocalRotation().z)
//		return;
//
//	if (!m_canCollide || !other.GetCollide())
//		return;
//
//	//Get Bounds of objects in question
//	Bound myB = GetBounds();
//	Bound otherB = other.GetBounds();
//	
//	//Get Coordinate points based on both objects
//	glm::vec2 projToOtherTop(myB.GetCentre().x, otherB.m_top);
//	glm::vec2 projToOtherBottem(myB.GetCentre().x, otherB.GetBottem());
//	glm::vec2 projToOtherLeft(otherB.m_left, myB.GetCentre().y);
//	glm::vec2 projToOtherRight(otherB.GetRight(), myB.GetCentre().y);
//
//	float overlap[4];
//	overlap[0] = abs(otherB.m_top - myB.GetCentre().y) - m_height / 2.0f; //Top Overlap
//	overlap[1] = abs(otherB.GetBottem() - myB.GetCentre().y) - m_height / 2.0f; //Bottem Overlap
//	overlap[2] = abs(otherB.m_left - myB.GetCentre().x) - m_width / 2.0f; //Left Overlap
//	overlap[3] = abs(otherB.GetRight() - myB.GetCentre().x) - m_width / 2.0f; //Right Overlap
//	
//	bool cond[4];
//	//Point exists in other but not within my bounds.
//	cond[0] = otherB.IsPointWithin(projToOtherTop);
//	cond[1] = otherB.IsPointWithin(projToOtherBottem);
//	cond[2] = otherB.IsPointWithin(projToOtherLeft);
//	cond[3] = otherB.IsPointWithin(projToOtherRight);
//
//	//Move me
//	for (int i = 0; i < 4; i++)
//	{
//		if (overlap[i] < -1 && cond[i])
//		{
//			switch (i)
//			{
//			case 0: //Top Overlap
//				m_position.y += cos(glm::radians(m_rotation.z)) * abs(overlap[i] + 1);
//				m_position.x += sin(glm::radians(m_rotation.z)) * abs(overlap[i] + 1);
//				break;
//			case 1: //Bottem Overlap
//				other.DisableCollision();
//				break;
//			case 2: //Left Overlap
//				m_position.x -= cos(glm::radians(m_rotation.z)) * abs(overlap[i] + 1);
//				m_position.y += sin(glm::radians(m_rotation.z)) * abs(overlap[i] + 1);
//				break;
//			case 3: //Right Overlap
//				m_position.x += cos(glm::radians(m_rotation.z)) * abs(overlap[i] + 1);
//				m_position.y += sin(glm::radians(m_rotation.z)) * abs(overlap[i] + 1);
//				break;
//			}
//		}
//	}
//
//	m_isGrounded = m_isGrounded || (overlap[0] < m_width / 2.0f && cond[0]); //Disable Gravity
//}

//	IsPointWithin( point )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Tests if a point is within this entity.
//		
//	Param:
//		-	glm::vec3	|	point to test.
//
//	Return: bool	|	status of if this point is within the entity.
//	
//bool Entity::IsPointWithin(glm::vec3 point)
//{
//	float dist = powf(glm::distance(m_position, point), 2);
//
//	if (dist < 2.0f * 2.0f)
//	{
//		return true;
//	}
//
//	return false;
//}