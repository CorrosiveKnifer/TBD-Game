

#include "Ball.h"

C_Ball::C_Ball(b2World* world, unsigned int playerID, sf::Vector2f _worldPosition, b2Vec2 _vectorVelocity) : Entity()
{
	Tx_MyBall.loadFromFile("Resources/images/Ball/Ball.png");

	Spr_Ball.setTexture(Tx_MyBall);
	Spr_Ball.setOrigin(Tx_MyBall.getSize().x/2.0f, Tx_MyBall.getSize().y / 2.0f);

	myPlayerID = playerID;
	if (myPlayerID == 1) { Spr_Ball.setColor(sf::Color::Red); }
	if (myPlayerID == 2) { Spr_Ball.setColor(sf::Color::Green); }
	if (myPlayerID == 3) { Spr_Ball.setColor(sf::Color(0, 150, 255, 255)); }
	if (myPlayerID == 4) { Spr_Ball.setColor(sf::Color::Yellow); }

	// create a position ahead of the overlay ball for creation.
	// _worldPosition = the position of the overlay ball sprite.
	// _vectorVelocity the normalised direction vector the ball should go, IE Up,Right vector = (1.0f, -1.0f)
	if (_vectorVelocity.x == 0 && _vectorVelocity.y == 0)
		_vectorVelocity = b2Vec2(1.0f, 0.0f);

	_vectorVelocity.Normalize();

	// place the ball out infront to avoid instantiating inside walls.
	_worldPosition.x += 20.0f * _vectorVelocity.x;
	_worldPosition.y += 20.0f * _vectorVelocity.y;

	//box2d setup
	MyBox2d.DEF.type = b2_dynamicBody;
	MyBox2d.DEF.position.Set(_worldPosition.x / C_GlobalVariables::PPM, _worldPosition.y / C_GlobalVariables::PPM);
	MyBox2d.DEF.bullet = true;
	MyBox2d.SHAPE.m_radius = (Tx_MyBall.getSize().x / 2.0f) / C_GlobalVariables::PPM;
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

	// Add speed modifier to the normalised direction vector.
	_vectorVelocity.x += _vectorVelocity.x * mf_SpeedModifier;
	_vectorVelocity.y += -_vectorVelocity.y * mf_SpeedModifier;

	m_bounceCount = m_bounceMax;
	MyBox2d.BOD->SetLinearVelocity(_vectorVelocity);
}

void C_Ball::Draw()
{
	sf::Color tempColor = this->Spr_Ball.getColor();
	float spacing = 1.1f;
	b2Vec2 trailVelocity = this->MyBox2d.BOD->GetLinearVelocity();
	sf::Vector2f originalPos = this->Spr_Ball.getPosition();
	float scale = 0.9f;
	// trail effect
	if (trailVelocity.x > 2.f || trailVelocity.x < -2.f || trailVelocity.y > 2.f || trailVelocity.y < -2.f) // limit the effect to higher than velocity amount -> 1?
	{
		for (unsigned int i = 0; i < 6; i++) // render 5 balls smaller, more transparent for trail effect.
		{
			this->Spr_Ball.setColor(sf::Color(tempColor.r, tempColor.g, tempColor.b, 255 - 110 - (i * i * i)));
			this->Spr_Ball.setScale(scale, scale);
			scale -= 0.1f;
			this->Spr_Ball.setPosition(originalPos.x + (-trailVelocity.x * (i + 1) * spacing), originalPos.y + (-trailVelocity.y * (i + 1) * spacing));
			Renderer::GetInstance().Draw(this->Spr_Ball);
		}
	}
	
	this->Spr_Ball.setPosition(originalPos);
	this->Spr_Ball.setScale(1.0f, 1.0f);
	this->Spr_Ball.setColor(tempColor);
	Renderer::GetInstance().Draw(this->Spr_Ball);

	Renderer::GetInstance().SetFontSize(25);
	Renderer::GetInstance().SetColour(sf::Color(255, 255, 255));
	Renderer::GetInstance().SetFontAlign(Align::Centre);
	Renderer::GetInstance().DrawTextToWorld(std::to_string(m_bounceCount), Spr_Ball.getPosition().x - 5, Spr_Ball.getPosition().y - 17.5);
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
	if (m_bounceCount == 0)
	{
		b2Vec2 velBefore = MyBox2d.BOD->GetLinearVelocity();
		MyBox2d.BOD->SetLinearVelocity(b2Vec2(0, velBefore.y));
	}
}

void C_Ball::HandleHit(Entity* other)
{
	if (other != nullptr)
	{
		// player scoring
		switch (myPlayerID)
		{
		case 1:
			C_GlobalVariables::Player_1_Score += 100;
			break;
		case 2:
			C_GlobalVariables::Player_2_Score += 100;
			break;
		case 3:
			C_GlobalVariables::Player_3_Score += 100;
			break;
		case 4:
			C_GlobalVariables::Player_4_Score += 100;
			break;
		}
	}

	if (m_bounceCount > 0)
	{
		m_bounceCount--;
		float ratio = (float)(m_bounceCount+1) / (m_bounceMax+1);
		b2Vec2 velBefore = MyBox2d.BOD->GetLinearVelocity();
		MyBox2d.BOD->SetLinearVelocity(b2Vec2(velBefore.x * ratio, velBefore.y * ratio));
	}	
}

C_Ball::~C_Ball()
{
	MyBox2d.BOD->GetWorld()->DestroyBody(MyBox2d.BOD);
}