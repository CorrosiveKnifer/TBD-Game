#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	Projectile.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	Projectile
// Description:
//		Sub class of entity which travels in a single direction.
//

//Parent Include
#include "Entity.h"

//Local Include
#include "ModelEntity.h"

class Projectile : public Entity
{
    //Member Functions:
public:
    Projectile();
    Projectile(std::string modelLoc, glm::vec3 _origin, glm::vec3 _direction, float _speed, unsigned int shooterIndex = 0);
    virtual ~Projectile();

    virtual void Draw(Camera* camera, Program* program);
    virtual void Process(float dT);

    void SetSize(float _width, float _height);
    void SetDirection(glm::vec3 _direction);
    void SetSpeed(float _speed);
    void SetFriendly(bool _friendly) { m_isFriendly = _friendly; };

    bool IsFriendly() { return m_isFriendly; };
    bool IsAlive() { return m_isAlive; };

    unsigned int GetShooterIndex() { return m_shooterIndex; };

protected:
    
private:

    //Member Data:
public:

protected:
    ModelEntity* m_pModel;

    float m_lifeTime;

    bool m_isAlive;
    bool m_isFriendly;
    unsigned int m_shooterIndex;
private:

};

#endif // !_PROJECTILE_H_