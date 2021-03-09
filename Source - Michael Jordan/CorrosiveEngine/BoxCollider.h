#ifndef _BOX_COLLIDER_H_
#define _BOX_COLLIDER_H_

#include "Collider.h"

class BoxCollider : public Collider
{
	//Member Functions:
public:
	BoxCollider();
	~BoxCollider();

	virtual void Draw(Camera* _cam);
	
protected:

private:

	//Member Data:
public:

protected:
	GLuint m_VAO;
private:

};

#endif // !_BOX_COLLIDER_H_

