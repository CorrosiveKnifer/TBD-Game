

#include "Ball.h"

C_Ball::C_Ball(b2World* world, unsigned int playerID, sf::Vector2f _worldPosition, b2Vec2 _vectorVelocity) : Entity()
{
	Tx_MyBall.loadFromFile("Resources/images/Ball/Ball.png");

	Spr_Ball.setTexture(Tx_MyBall);
	Spr_Ball.setOrigin(Tx_MyBall.getSize().x/2.0f, Tx_MyBall.getSize().y / 2.0f);

	myPlayerID = playerID;
	
	// create a position ahead of the overlay ball for creation.
	// _worldPosition = the position of the overlay ball sprite.
	// _vectorVelocity the normalised direction vector the ball should go, IE Up,Right vector = (1.0f, -1.0f)
	if (_vectorVelocity.x == 0 && _vectorVelocity.y == 0)
		_vectorVelocity = b2Vec2(1.0f, 0.0f);

	_vectorVelocity.Normalize();

	//box2d setup
	MyBox2d.DEF.type = b2_dynamicBody;
	MyBox2d.DEF.position.Set(_worldPosition.x / C_GlobalVariables::PPM, _worldPosition.y / C_GlobalVariables::PPM);  // use spawn points for this.
	MyBox2d.DEF.bullet = true;
	MyBox2d.SHAPE.m_radius = (Tx_MyBall.getSize().x / 2.0f) / C_GlobalVariables::PPM;
	MyBox2d.FIX.shape = &MyBox2d.SHAPE;
	MyBox2d.FIX.density = 0.5f;
	MyBox2d.FIX.friction = 0.5f;
	MyBox2d.FIX.restitution = 0.9f;
	MyBox2d.FIX.restitutionThreshold = 0.1f;
	MyBox2d.FIX.filter.categoryBits = C_GlobalVariables::GetCategoryFor(playerID);
	MyBox2d.FIX.filter.maskBits = C_GlobalVariables::GetLayerMaskFor(playerID);
	MyBox2d.FIX.userData.pointer = reinterpret_cast<uintptr_t>(this);
	MyBox2d.BOD = world->CreateBody(&MyBox2d.DEF);
	MyBox2d.BOD->CreateFixture(&MyBox2d.FIX);

	// Add speed modifier to the normalised direction vector.
	_vectorVelocity.x += _vectorVelocity.x * mf_SpeedModifier;
	_vectorVelocity.y += -_vectorVelocity.y * mf_SpeedModifier;

	MyBox2d.BOD->SetLinearVelocity(_vectorVelocity);
}

void C_Ball::Draw()
{
	Renderer::GetInstance().Draw(Spr_Ball);
}

void C_Ball::Process(float dT)
{
	this->Spr_Ball.setPosition(this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM, this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM);

	//Wrap arround
	if (this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM > C_GlobalVariables::ScreenSizeY)
	{
		this->MyBox2d.BOD->SetTransform(b2Vec2(this->MyBox2d.BOD->GetPosition().x, 0), 0);
	}
	else if (this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM < 0)
	{
		this->MyBox2d.BOD->SetTransform(b2Vec2(this->MyBox2d.BOD->GetPosition().x, C_GlobalVariables::ScreenSizeY / C_GlobalVariables::PPM), 0);
	}
}

void C_Ball::HandleHit()
{
	MyBox2d.BOD->SetLinearVelocity(b2Vec2(0, 0));
}

C_Ball::~C_Ball()
{
	MyBox2d.BOD->GetWorld()->DestroyBody(MyBox2d.BOD);
}