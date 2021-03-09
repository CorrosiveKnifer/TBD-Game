#ifndef _COLLIDER_H_
#define _COLLIDER_H_

#include "Transform.h"
#include "Renderer.h"
#include "Program.h"

class Camera;

class Collider
{
	//Member Functions:
public:
	Collider() { m_program = &Renderer::GetInstance().CreateProgram("Debug.vs", "Debug.fs"); };
	~Collider() {};

	virtual void Draw(Camera* _cam) = 0;

	void SetPosition(glm::vec3 _pos) { m_transform.SetPosition(_pos); };
	void SetRotation(glm::quat _rot) { m_transform.SetRotation(_rot); };
	void SetScale(glm::vec3 _scale) { m_transform.SetScale(_scale); };
protected:

private:

	//Member Data:
public:

protected:
	Transform m_transform;
	Program* m_program;
private:

};

#endif // !_COLLIDER_H_
