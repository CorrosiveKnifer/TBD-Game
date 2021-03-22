#ifndef _ENTITY_H_
#define _ENTITY_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Entity.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Entity
// Description:
//		An entity is a physical object within the world which can be manipulated
//		by either a user or background code.
//

//Parent Include
#include "Transform.h"
#include "Program.h"
#include "Component.h"

#include <vector>
//Structure bounds
//Used for 2D collision

class Collider;

struct Bound
{
	Bound(float _Left, float _Top, float _Width, float _Height)
	{
		m_left = _Left;
		m_top = _Top;
		m_width = _Width;
		m_height = _Height;
	}

	float m_left;
	float m_top;
	float m_width;
	float m_height;

	float GetRight() { return m_left + m_width; };
	float GetBottem() { return m_top + m_height; };
	glm::vec2 GetCentre() { return glm::vec2(m_left + m_width / 2.0f, m_top + m_height / 2.0f); };

	bool IsPointWithin(glm::vec2 _point) 
	{
		bool xAxis = _point.x >= m_left && _point.x <= GetRight();
		bool yAxis = _point.y <= m_top && _point.y >= GetBottem();

		return xAxis && yAxis;
	};
};

class Entity
{
	//Member Functions:
public:
	Entity();
	virtual ~Entity();

	virtual void Draw(Camera* camera, Program* program);
	virtual void Process(float dT);

	virtual std::string Deserialize(std::string update);

	Transform GetTransform() const { return m_localTransform; };
	Transform& GetTransformRef() { return m_localTransform; };

	void SetInheritMatrix(glm::mat4 _inherit) { m_inheritMatrix = _inherit; };
	bool AddChild(Entity* _child);
	bool AddComponent(Component* _component);
protected:

private:

	//Member Data:
public:
	
protected:
	std::vector<Entity*> m_subEntities;
	std::vector<Component*> m_components;

	//Collider* m_collider;
	Transform m_localTransform;
	glm::mat4 m_inheritMatrix;

private:
	
};

#endif // !_ENTITY_H_
