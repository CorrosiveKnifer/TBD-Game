

#include "Ball.h"

C_Ball::C_Ball(b2World* world, C_Player* _ownerOfThisBall, b2Vec2 _worldPosition, b2Vec2 _vectorVelocity) : Entity()
{
	Tx_MyBall.loadFromFile("Resources/images/Ball/Ball.png");

	Spr_Ball.setTexture(Tx_MyBall);
	Spr_Ball.setOrigin(Tx_MyBall.getSize().x/2.0f, Tx_MyBall.getSize().y / 2.0f);

	
	// create a position ahead of the overlay ball for creation.
	// _worldPosition = the position of the overlay ball sprite.
	// _vectorVelocity the normalised direction vector the ball should go, IE Up,Right vector = (1.0f, -1.0f)
	float tempX = _worldPosition.x + (_vectorVelocity.x *60.f); // this number(60) might need increasing - need to test.
	float tempY = _worldPosition.y + (_vectorVelocity.y * 60.f);;

	//box2d setup
	MyBox2d.DEF.type = b2_dynamicBody;
	MyBox2d.DEF.position.Set(tempX / C_GlobalVariables::PPM, tempY / C_GlobalVariables::PPM);  // use spawn points for this.
	MyBox2d.SHAPE.m_radius = (Tx_MyBall.getSize().x / 2.0f) / C_GlobalVariables::PPM;
	MyBox2d.FIX.shape = &MyBox2d.SHAPE;
	MyBox2d.FIX.density = 0.5f;
	MyBox2d.FIX.friction = 0.5f;
	MyBox2d.FIX.restitution = 0.9f;
	MyBox2d.FIX.restitutionThreshold = 0.1f;
	MyBox2d.BOD = world->CreateBody(&MyBox2d.DEF);
	MyBox2d.BOD->CreateFixture(&MyBox2d.FIX);

	// Add speed modifier to the normalised direction vector.
	_vectorVelocity.x += _vectorVelocity.x * mf_SpeedModifier;
	_vectorVelocity.y += _vectorVelocity.y * mf_SpeedModifier;

	MyBox2d.BOD->SetLinearVelocity(_vectorVelocity);
}

void C_Ball::Draw()
{

}

void C_Ball::Process(float dT)
{
	this->Spr_Ball.setPosition(this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM, this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM);
}

C_Ball::~C_Ball()
{

}