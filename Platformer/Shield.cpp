#include "Shield.h"

Shield::Shield(b2World* world, unsigned int playerID, b2Vec2 _worldPosition)
{
	m_bodySprite = Renderer::GetInstance().CreateSprite("images/powerups/shield.png");
	m_bodySprite->setOrigin(m_bodySprite->getTexture()->getSize().x/2, m_bodySprite->getTexture()->getSize().y/2);
	m_bodySprite->setScale(0.6f, 0.6f);

	//box2d setup
	MyBox2d.DEF.type = b2_staticBody;
	MyBox2d.DEF.position.Set(_worldPosition.x, _worldPosition.y);
	MyBox2d.DEF.bullet = true;
	MyBox2d.SHAPE.m_radius = ((m_bodySprite->getTexture()->getSize().x/2) * 0.6f) / C_GlobalVariables::PPM;
	MyBox2d.FIX.shape = &MyBox2d.SHAPE;
	MyBox2d.FIX.density = 0.5f;
	MyBox2d.FIX.friction = 0.5f;
	MyBox2d.FIX.restitution = 0.9f;
	MyBox2d.FIX.restitutionThreshold = 0.4f;
	MyBox2d.FIX.filter.categoryBits = C_GlobalVariables::GetCategoryFor(playerID);
	MyBox2d.FIX.filter.maskBits = C_GlobalVariables::GetLayerMaskFor(playerID);
	MyBox2d.FIX.userData.pointer = reinterpret_cast<uintptr_t>(this);
	MyBox2d.BOD = world->CreateBody(&MyBox2d.DEF);
	MyBox2d.BOD->CreateFixture(&MyBox2d.FIX);
}

Shield::~Shield()
{
	MyBox2d.BOD->GetWorld()->DestroyBody(MyBox2d.BOD);

	delete m_bodySprite;
	m_bodySprite = 0;
}

void Shield::Draw()
{
	Renderer::GetInstance().Draw(*m_bodySprite);
}

void Shield::Process(float dT)
{
	m_bodySprite->setPosition(this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM, this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM);
}
