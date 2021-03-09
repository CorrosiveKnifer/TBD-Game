#ifndef _SCENE_H_
#define _SCENE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	SceneADT.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	SceneADT (Abstract)
// Description:
//		An abstract data type for decending scene class that might be created.
//		This class allows the GLManager to switch between scene classes.
//

//Dependeny Includes
#include <glew.h>
#include <freeglut.h>
#include <vector>

#include "Renderer.h"
#include "LoadManager.h"
#include "Entity.h"
#include "Program.h"
#include "Camera.h"
#include "LogManager.h"

//
// SceneObject is an entity with the scene which is automatically rendered and
// processed. Containing additional boolean flags to handle deconstruction.
// Note: IsAlive flag is only changed when the SceneADT::DestroyEntity Function is called.
//
struct SceneObject
{
	SceneObject(Entity* _entity, bool _alive = true, bool _deconst = false) 
	{
		entity = _entity;
		isAlive = _alive;
		isDeconstructed = _deconst;
	};

	Entity* entity;
	bool isAlive = true;
	bool isDeconstructed = false;
};

class SceneADT
{
public:
	//Destructor
	virtual ~SceneADT()
	{
		m_pProgram = 0;

		//Loop through all objects to remove any which are not alive
		std::vector<SceneObject>::iterator iter = m_pEntities.begin();
		while (iter != m_pEntities.end())
		{
			//Check if this scene is taking responsibility for removal.
			if (!(*iter).isDeconstructed)
				delete (*iter).entity;

			iter = m_pEntities.erase(iter);
		}
	};

	//Abstract functions:
	virtual bool Initialise() = 0;

	virtual void PreLoad() = 0;

	virtual void Draw() = 0;

	virtual void Process(float dT) = 0;

	virtual void Resize() = 0;

	virtual void InstantiateEntity(Entity* e) { m_pEntities.push_back(SceneObject(e)); };

	virtual void DestroyEntity(Entity* other, bool deconstruct = false) 
	{
		for (SceneObject object : m_pEntities)
		{
			if (&(*object.entity) == &(*other))
			{
				object.isAlive = false; //Set to remove at the next process
				object.isDeconstructed = deconstruct;
				return;
			}
		}
		LogManager::GetInstance().LogErr("Entity tried to be removed but didn't exist.");
	}
protected:
	//Default Constructor
	SceneADT() { m_pProgram = &Renderer::GetInstance().CreateDefaultProgram(); };
	
	void ProcessEntities(float dT) 
	{
		//Loop through all objects to remove any which are not alive
		std::vector<SceneObject>::iterator iter = m_pEntities.begin();
		while (iter != m_pEntities.end())
		{
			if (!(*iter).isAlive)
			{
				//Check if this scene is taking responsibility for removal.
				if (!(*iter).isDeconstructed)
					delete (*iter).entity;

				iter = m_pEntities.erase(iter);
			}
			else
				iter++;
		}

		for (unsigned int i = 0; i < m_pEntities.size(); i++)
		{
			if(m_pEntities.at(i).isAlive)
				m_pEntities.at(i).entity->Process(dT);
		}
	};

	void DrawEntities(Camera* _cam) {
		std::vector<SceneObject>::iterator iter = m_pEntities.begin();
		while (iter != m_pEntities.end())
		{
			if((*iter).isAlive)
				(*iter).entity->Draw(_cam, m_pProgram);
			iter++;
		}
	};

private:
	
	//Member Data:
public:

protected:
	Program* m_pProgram;

	std::vector<SceneObject> m_pEntities;
private:

};

#endif // !_SCENE_H_
