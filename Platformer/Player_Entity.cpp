// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: Player_Entity.cpp
// Description   	: Player Class for any player.
// Author         	: Sonja Fowler
// Mail         	: sonja@alp.co.nz
//
#include"Player_Entity.h"

C_Player::C_Player(b2World* world,int _playerNumber, b2Vec2 _position) : Entity()
{
	PlayerNumber = _playerNumber;
	std::string tempPath = "";
	//1,2,3,4  1=Red, 2=Green, 3= Blue,4=Yellow.
	if (PlayerNumber == 1)
	{
		tempPath = "Resources/images/Characters/Finals_Red_Sized/";
		myBallColor = sf::Color::Red;
	}
	if (PlayerNumber == 2)
	{
		tempPath = "Resources/images/Characters/Finals_GREEN_Sized/";
		myBallColor = sf::Color::Green;
	}
	if (PlayerNumber == 3)
	{
		tempPath = "Resources/images/Characters/Finals_BLUE_Sized/";
		myBallColor = sf::Color::Blue;
	}
	if (PlayerNumber == 4)
	{
		tempPath = "Resources/images/Characters/Finals_YELLOW_sized/";
		myBallColor = sf::Color::Yellow;
	}

	//Renderer can do this for you.
	Tx_LegsIdle.loadFromFile(tempPath + "_0013_Legs_Idle.png");
	Tx_LegsJump.loadFromFile(tempPath + "_0012_Legs_Jump.png");
	Tx_LegsRun1.loadFromFile(tempPath + "_0009_Legs_Run_1.png");
	Tx_LegsRun2.loadFromFile(tempPath + "_0011_Legs_Run_2.png");
	Tx_LegsRun3.loadFromFile(tempPath + "_0010_Legs_Run_3.png");
	Tx_LegsRun4.loadFromFile(tempPath + "_0011_Legs_Run_4.png");

	Tx_UB_ThrowUp1.loadFromFile(tempPath + "_0001_Throw_Up_1.png");
	Tx_UB_ThrowUp2.loadFromFile(tempPath + "_0000_Throw_Up_2.png");
	Tx_UB_ThrowDown1.loadFromFile(tempPath + "_0003_Throw_Down_1.png");
	Tx_UB_ThrowDown2.loadFromFile(tempPath + "_0002_Throw_Down_2.png");
	Tx_UB_ThrowSide1.loadFromFile(tempPath + "_0016_Throw_Mid_1.png");
	Tx_UB_ThrowSide2.loadFromFile(tempPath + "_0015_Throw_Mid_2.png");
	Tx_UB_ThrowDiagUp1.loadFromFile(tempPath + "_0005_Throw_DiagUp_1.png");
	Tx_UB_ThrowDiagUp2.loadFromFile(tempPath + "_0004_Throw_DiagUp_2.png");

	Tx_UB_ThrowDiagDown1.loadFromFile(tempPath + "_0007_Throw_DiagDown_1.png");
	Tx_UB_ThrowDiagDown2.loadFromFile(tempPath + "_0006_Throw_DiagDown_2.png");
	Tx_UB_Shield.loadFromFile(tempPath + "_0008_Shield.png");
	Tx_UB_Victory.loadFromFile(tempPath + "_0014_Victory.png");

	Tx_MyBall_Overlay.loadFromFile("Resources/images/Ball/Ball.png");

	// sprite setup
	Spr_Legs.setTexture(Tx_LegsIdle);
	Spr_UpperBody.setTexture(Tx_UB_ThrowSide1);
	Spr_Legs.setOrigin(41.0f, 82.0f);
	Spr_UpperBody.setOrigin(41.0f, 82.0f);

	Spr_Ball_overlay.setTexture(Tx_MyBall_Overlay);
	Spr_Ball_overlay.setOrigin(Tx_MyBall_Overlay.getSize().x / 2.0f, Tx_MyBall_Overlay.getSize().y / 2.0f);
	Spr_Ball_overlay.setColor(myBallColor);

	//box2d setup
	MyBox2d.DEF.type = b2_dynamicBody;
	MyBox2d.DEF.position.Set(_position.x / C_GlobalVariables::PPM, _position.y / C_GlobalVariables::PPM);  // use spawn points for this.
	MyBox2d.SHAPE.SetAsBox(15 / C_GlobalVariables::PPM, 50 / C_GlobalVariables::PPM);
	MyBox2d.FIX.shape = &MyBox2d.SHAPE;
	MyBox2d.FIX.density = 1.0f;
	MyBox2d.FIX.friction = 1.0f;
	MyBox2d.FIX.restitution = 0.001f;
	MyBox2d.FIX.restitutionThreshold = 10.f;
	MyBox2d.BOD = world->CreateBody(&MyBox2d.DEF);
	MyBox2d.BOD->CreateFixture(&MyBox2d.FIX);

	MyDirection = facing_right;
	MyMovingDirection = moving_none;
}

void C_Player::Draw()
{
	// draw legs, upperBody, ball.
	Renderer::GetInstance().Draw(Spr_UpperBody);
	Renderer::GetInstance().Draw(Spr_Legs);
	// Ball overlay while being held by player
	if (this->mb_PlayerHasBall == true)
	{
		// draw ball being held
	}
}

void C_Player::Process(float dT)
{
	this->Spr_Legs.setPosition(this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM, this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM);
	this->Spr_UpperBody.setPosition(this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM, this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM);

	// optimize ->  this->MyBox2d.BOD->GetLinearVelocity().x  and for y

	if (this->MyBox2d.BOD->GetLinearVelocity().x < -C_GlobalVariables::minimumSpeedForAnim)
	{
		this->Spr_Legs.setTexture(this->Tx_LegsRun1);
		this->Spr_Legs.setScale(-1.0f, 1.0f);
		// setup running based on speed
	}
	if (this->MyBox2d.BOD->GetLinearVelocity().x > C_GlobalVariables::minimumSpeedForAnim)
	{
		this->Spr_Legs.setTexture(this->Tx_LegsRun1);
		this->Spr_Legs.setScale(1.0f, 1.0f);
		// setup running based on speed
	}

	// stationary = Idle legs.
	if (this->MyBox2d.BOD->GetLinearVelocity().x > -C_GlobalVariables::minimumSpeedForAnim && this->MyBox2d.BOD->GetLinearVelocity().x < C_GlobalVariables::minimumSpeedForAnim)
	{
		this->Spr_Legs.setTexture(this->Tx_LegsIdle);
		this->Spr_Legs.setScale(1.0f, 1.0f);
		
	}
	// final -  override for legs if we are airborne
	if (this->MyBox2d.BOD->GetLinearVelocity().y > C_GlobalVariables::minimumSpeedForAnimJump || this->MyBox2d.BOD->GetLinearVelocity().y < -C_GlobalVariables::minimumSpeedForAnimJump)
	{
		this->Spr_Legs.setTexture(this->Tx_LegsJump);
		if (this->MyBox2d.BOD->GetLinearVelocity().x > C_GlobalVariables::minimumSpeedForAnim)
		{
			this->Spr_Legs.setScale(1.0f, 1.0f);
		}
		else
		{
			this->Spr_Legs.setScale(-1.0f, 1.0f);
		}
	}

	this->Spr_UpperBody.setScale((FaceDirection.x >= 0 ? 1.0f : -1.0f), 1.0f);

	switch (MyDirection)
	{
	case C_Player::facing_right:
		this->Spr_UpperBody.setTexture(this->Tx_UB_ThrowSide1);
		break;
	case C_Player::facing_upRight:
		this->Spr_UpperBody.setTexture(this->Tx_UB_ThrowDiagUp1);
		break;
	case C_Player::facing_up:
		this->Spr_UpperBody.setTexture(this->Tx_UB_ThrowUp1);
		break;
	case C_Player::facing_upLeft:
		this->Spr_UpperBody.setTexture(this->Tx_UB_ThrowDiagUp1);
		break;
	case C_Player::facing_left:
		this->Spr_UpperBody.setTexture(this->Tx_UB_ThrowSide1);
		break;
	case C_Player::facing_downLeft:
		this->Spr_UpperBody.setTexture(this->Tx_UB_ThrowDiagDown1);
		break;
	case C_Player::facing_down:
		this->Spr_UpperBody.setTexture(this->Tx_UB_ThrowDown1);
		break;
	case C_Player::facing_downRight:
		this->Spr_UpperBody.setTexture(this->Tx_UB_ThrowDiagDown1);
		break;
	default:
		break;
	}

	// Upper body sprite



	// Ball overlay while being held by player
	if (this->mb_PlayerHasBall == true)
	{
		switch (MyDirection)
		{
		case C_Player::facing_right:
			this->Spr_Ball_overlay.setPosition(this->Spr_UpperBody.getPosition().x - 27.f, this->Spr_UpperBody.getPosition().y - 23.f);
			break;
		case C_Player::facing_upRight:
			this->Spr_Ball_overlay.setPosition(this->Spr_UpperBody.getPosition().x - 25.f, this->Spr_UpperBody.getPosition().y - 4.f);
			break;
		case C_Player::facing_up:
			this->Spr_Ball_overlay.setPosition(this->Spr_UpperBody.getPosition().x - 1.f, this->Spr_UpperBody.getPosition().y + 10.f);
			break;
		case C_Player::facing_upLeft:
			this->Spr_Ball_overlay.setPosition(this->Spr_UpperBody.getPosition().x + 25.f, this->Spr_UpperBody.getPosition().y - 4.f);
			break;
		case C_Player::facing_left:
			this->Spr_Ball_overlay.setPosition(this->Spr_UpperBody.getPosition().x + 27.f, this->Spr_UpperBody.getPosition().y - 23.f);
			break;
		case C_Player::facing_downLeft:
			this->Spr_Ball_overlay.setPosition(this->Spr_UpperBody.getPosition().x + 35.f, this->Spr_UpperBody.getPosition().y - 44.f);
			break;
		case C_Player::facing_down:
			this->Spr_Ball_overlay.setPosition(this->Spr_UpperBody.getPosition().x - 1.f, this->Spr_UpperBody.getPosition().y - 48.f);
			break;
		case C_Player::facing_downRight:
			this->Spr_Ball_overlay.setPosition(this->Spr_UpperBody.getPosition().x - 35.f, this->Spr_UpperBody.getPosition().y - 44.f);
			break;
		default:
			break;
		}
	}
	
}

C_Player::~C_Player()
{

}