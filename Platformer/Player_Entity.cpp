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

#include "Player_Entity.h"
#include "RayCastClass.h"
#include <iostream>

C_Player::C_Player(b2World* world,int _playerNumber, b2Vec2 _position) : Entity()
{
	PlayerNumber = _playerNumber;
	std::string tempPath = "";
	//1,2,3,4  1=Red, 2=Green, 3= Blue,4=Yellow.
	if (PlayerNumber == 1)
	{
		tempPath = "Resources/images/Characters/Finals_Red_Sized/";
		myBallColor = sf::Color::Red;
		myScore = C_GlobalVariables::Player_1_Score;
	}
	if (PlayerNumber == 2)
	{
		tempPath = "Resources/images/Characters/Finals_GREEN_Sized/";
		myBallColor = sf::Color::Green;
		myScore = C_GlobalVariables::Player_2_Score;
	}
	if (PlayerNumber == 3)
	{
		tempPath = "Resources/images/Characters/Finals_BLUE_Sized/";
		myBallColor = sf::Color(0, 150, 255, 255);
		myScore = C_GlobalVariables::Player_3_Score;
	}
	if (PlayerNumber == 4)
	{
		tempPath = "Resources/images/Characters/Finals_YELLOW_sized/";
		myBallColor = sf::Color::Yellow;
		myScore = C_GlobalVariables::Player_4_Score;
	}

	//Renderer can do this for you.
	Tx_LegsIdle.loadFromFile(tempPath + "_0013_Legs_Idle.png");
	Tx_LegsJump.loadFromFile(tempPath + "_0012_Legs_Jump.png");
	Tx_LegsRun[0].loadFromFile(tempPath + "_0009_Legs_Run_1.png");
	Tx_LegsRun[1].loadFromFile(tempPath + "_0011_Legs_Run_2.png");
	Tx_LegsRun[2].loadFromFile(tempPath + "_0010_Legs_Run_3.png");
	Tx_LegsRun[3].loadFromFile(tempPath + "_0011_Legs_Run_4.png");

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

 	Spr_Emote = Renderer::GetInstance().CreateSprite("images/Emotes/Emote3.png");
	Spr_Emote->setOrigin(Spr_Emote->getTexture()->getSize().x / 2.0f, Spr_Emote->getTexture()->getSize().y / 2.0f);
	Spr_Emote->setScale(0.45, 0.45);

	//box2d setup
	MyBox2d.DEF.type = b2_dynamicBody;
	MyBox2d.DEF.fixedRotation = true;
	MyBox2d.DEF.position.Set(_position.x / C_GlobalVariables::PPM, _position.y / C_GlobalVariables::PPM);  // use spawn points for this.
	MyBox2d.SHAPE.SetAsBox(15 / C_GlobalVariables::PPM, 50 / C_GlobalVariables::PPM);
	MyBox2d.FIX.shape = &MyBox2d.SHAPE;
	MyBox2d.FIX.density = 1.5f;
	MyBox2d.FIX.friction = 1.0f;
	MyBox2d.FIX.restitution = 0.001f;
	MyBox2d.FIX.userData.pointer = reinterpret_cast<uintptr_t>(this);
	MyBox2d.FIX.restitutionThreshold = 10.f;
	MyBox2d.FIX.filter.categoryBits = C_GlobalVariables::GetCategoryFor(PlayerNumber);
	MyBox2d.FIX.filter.maskBits = C_GlobalVariables::GetLayerMaskFor(PlayerNumber);
	MyBox2d.BOD = world->CreateBody(&MyBox2d.DEF);
	MyBox2d.BOD->CreateFixture(&MyBox2d.FIX);

	MyDirection = facing_right;
	MyMovingDirection = moving_none;

	m_immuneTimer = 2.0f;

	//myPowerupType = NONE;
	Tx_Emotes = new sf::Texture[4];
	Tx_Emotes[0] = *Renderer::GetInstance().CreateTexture("images/Emotes/Emote1.png");
	Tx_Emotes[1] = *Renderer::GetInstance().CreateTexture("images/Emotes/Emote2.png");
	Tx_Emotes[2] = *Renderer::GetInstance().CreateTexture("images/Emotes/Emote3.png");
	Tx_Emotes[3] = *Renderer::GetInstance().CreateTexture("images/Emotes/Emote4.png");
}

void C_Player::Draw()
{
	if (m_isDead)
	{
		return;
	}

	//Movement speed blurr
	if (m_playerSpeedMod > 1.0f)
	{
		Spr_UpperBody.setColor(sf::Color(255, 255, 255, 127.5));
		Spr_Legs.setColor(sf::Color(255, 255, 255, 127.5));

		sf::Transform t;
		t.translate(sf::Vector2f(2.5f, 0.0f));
		Renderer::GetInstance().PushTransform(t);
		Renderer::GetInstance().Draw(Spr_UpperBody);
		Renderer::GetInstance().Draw(Spr_Legs);
		Renderer::GetInstance().PushTransform(t);
		Renderer::GetInstance().Draw(Spr_UpperBody);
		Renderer::GetInstance().Draw(Spr_Legs);
		Renderer::GetInstance().PopTransform(t);
		Renderer::GetInstance().PopTransform(t);

		Spr_UpperBody.setColor(sf::Color(255, 255, 255, 255));
		Spr_Legs.setColor(sf::Color(255, 255, 255, 255));
	}

	// draw legs, upperBody, ball.
	Renderer::GetInstance().Draw(Spr_UpperBody);
	Renderer::GetInstance().Draw(Spr_Legs);

	if (MyBall != nullptr)
	{
		MyBall->Draw();
	}
	else
	{
		
		Renderer::GetInstance().Draw(Spr_Ball_overlay);
	}

	if (m_emoteTimer > 0)
	{
		float ratio = 1.0f;
		if (m_emoteTimer >= 1.0f)
		{
			ratio = 1.0f - (m_emoteTimer - 1.0f) / 0.5f;
		}
		if (m_emoteTimer <= 0.5f)
		{
			ratio = m_emoteTimer / 0.5f;
		}
		Spr_Emote->setColor(sf::Color(255, 255, 255, 255 * ratio));
		Renderer::GetInstance().Draw(*Spr_Emote);
	}

	// Ball overlay while being held by player
	if (this->mb_PlayerHasBall == true)
	{
		// draw ball being held
	}
}

void C_Player::Process(float dT)
{
	// grab any score accumulated on global variables (from ball collisions hits) will sort multi level later.
	switch (PlayerNumber)
	{
	case 1:
		myScore += C_GlobalVariables::Player_1_Score;
		C_GlobalVariables::Player_1_Score = 0; // reset this during level play, set at end of level for next if points accumulating
		break;
	case 2:
		myScore += C_GlobalVariables::Player_2_Score;
		C_GlobalVariables::Player_2_Score = 0; // reset this during level play, set at end of level for next if points accumulating
		break;
	case 3:
		myScore += C_GlobalVariables::Player_3_Score;
		C_GlobalVariables::Player_3_Score = 0; // reset this during level play, set at end of level for next if points accumulating
		break;
	case 4:
		myScore += C_GlobalVariables::Player_4_Score;
		C_GlobalVariables::Player_4_Score = 0; // reset this during level play, set at end of level for next if points accumulating
		break;
	}
	if (m_powerUpTimer > 0)
		m_powerUpTimer = std::clamp(m_powerUpTimer - dT, 0.0f, m_powerUpTimer);

	if (m_emoteTimer > 0)
		m_emoteTimer = std::clamp(m_emoteTimer - dT, 0.0f, m_emoteTimer);

	if (m_playerSpeedMod >= 1.0f)
	{
		m_playerSpeedMod = 1.0f + 1.0f * (m_powerUpTimer / m_powerUpTimerMax);
	}

	if(MyBall != nullptr)
		MyBall->Process(dT);

	if (m_isDead )
	{
		m_deathTimer += dT;
		if (MyBox2d.BOD != nullptr)
		{
			MyBox2d.BOD->GetWorld()->DestroyBody(MyBox2d.BOD);
			MyBox2d.BOD = nullptr;
		}
		//Destroy ball when dead
		//if (MyBall != nullptr)
		//{
		//	delete MyBall;
		//	MyBall = nullptr;
		//}
		return;
	}

	HandleInput(dT);
	ProcessImmuneFrames(dT);
	
	this->Spr_Legs.setPosition(this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM, this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM);
	this->Spr_UpperBody.setPosition(this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM, this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM);
	Spr_Emote->setPosition(this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM, this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM - 100);
	
	RayCastClass RayResult;
	MyBox2d.BOD->GetWorld()->RayCast(&RayResult, MyBox2d.BOD->GetPosition(), MyBox2d.BOD->GetPosition() + b2Vec2(0, 2));
	m_isGrounded = RayResult.rayHits.size() > 0;

	//Wrap arround
	if (this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM > C_GlobalVariables::ScreenSizeY)
	{
		this->MyBox2d.BOD->SetTransform(b2Vec2(this->MyBox2d.BOD->GetPosition().x, 0), 0);
	}
	else if (this->MyBox2d.BOD->GetPosition().y * C_GlobalVariables::PPM < 0)
	{
		this->MyBox2d.BOD->SetTransform(b2Vec2(this->MyBox2d.BOD->GetPosition().x, C_GlobalVariables::ScreenSizeY / C_GlobalVariables::PPM), 0);
	}

	if (this->MyBox2d.BOD->GetLinearVelocity().x < -C_GlobalVariables::minimumSpeedForAnim)
	{
		// setup running based on speed
		mf_Anim_RunSpeed_Timer += dT;
		if (mf_Anim_RunSpeed_Timer > mf_Anim_RunSpeed)
		{
			mi_Current_Leg_Frame++;
			mf_Anim_RunSpeed_Timer = 0.0f;
		}
		if (mi_Current_Leg_Frame > 3) { mi_Current_Leg_Frame = 0; }
		this->Spr_Legs.setTexture(this->Tx_LegsRun[mi_Current_Leg_Frame]);
		this->Spr_Legs.setScale(-1.0f, 1.0f);
		
	}
	if (this->MyBox2d.BOD->GetLinearVelocity().x > C_GlobalVariables::minimumSpeedForAnim)
	{
		// setup running based on speed
		mf_Anim_RunSpeed_Timer += dT;
		if (mf_Anim_RunSpeed_Timer > mf_Anim_RunSpeed)
		{
			mi_Current_Leg_Frame++;
			mf_Anim_RunSpeed_Timer = 0.0f;
		}
		if (mi_Current_Leg_Frame > 3) { mi_Current_Leg_Frame = 0; }
		this->Spr_Legs.setTexture(this->Tx_LegsRun[mi_Current_Leg_Frame]);
		this->Spr_Legs.setScale(1.0f, 1.0f);
		
	}

	// stationary = Idle legs.
	if (this->MyBox2d.BOD->GetLinearVelocity().x > -C_GlobalVariables::minimumSpeedForAnim && this->MyBox2d.BOD->GetLinearVelocity().x < C_GlobalVariables::minimumSpeedForAnim)
	{
		if (MyDirection == C_Player::facing_right || MyDirection == C_Player::facing_upRight || MyDirection == C_Player::facing_downRight || MyDirection == C_Player::facing_down || MyDirection == C_Player::facing_up)
		{
			this->Spr_Legs.setTexture(this->Tx_LegsIdle);
			this->Spr_Legs.setScale(1.0f, 1.0f);
		}
		else
		{
			this->Spr_Legs.setTexture(this->Tx_LegsIdle);
			this->Spr_Legs.setScale(-1.0f, 1.0f);
		}
		
		//reset
		mi_Current_Leg_Frame = 0;
		mf_Anim_RunSpeed_Timer = 0.0f;
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
		//reset
		mi_Current_Leg_Frame = 0;
		mf_Anim_RunSpeed_Timer = 0.0f;
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

void C_Player::HandleHit(Entity* other)
{
	if (!m_isDead && !IsImmune() && !other->IsImmune())
	{
		m_isDead = true;
		m_deathTimer = 0.0f;
		this->myLives--;
	}
}

void C_Player::Respawn(b2Vec2 position, b2World* world)
{
	m_immuneTimer = 3.0f;
	MyBox2d.BOD = world->CreateBody(&MyBox2d.DEF);
	MyBox2d.BOD->CreateFixture(&MyBox2d.FIX);
	MyBox2d.BOD->SetTransform(position, 0);

	m_isDead = false;

	if (MyBall != nullptr)
	{
		delete MyBall;
		MyBall = nullptr;
	}
}

void C_Player::ApplyPowerUp(PowerUpType type)
{
	switch (type)
	{
	case NONE:
		break;
	case SPEED:
		myPowerupType = SPEED;
		break;
	case TRIPLESHOT:
		myPowerupType = TRIPLESHOT;
		break;
	case SHIELD:
		myPowerupType = SHIELD;
		break;
	case RAILSHOT:
		myPowerupType = RAILSHOT;
		break;
	case WATERFALL:
		myPowerupType = WATERFALL;
		break;
	default:
		break;
	}
}

void C_Player::HandleInput(float dt)
{
	float xVelocity = MyBox2d.BOD->GetLinearVelocity().x * 0.8f; //0.8 for friction
	if (xVelocity <= 0.05f && xVelocity >= -0.05f)
	{
		xVelocity = 0.0f;
	}
	float yVelocity = MyBox2d.BOD->GetLinearVelocity().y;
	if (yVelocity <= 0.05f && yVelocity >= -0.05f)
	{
		yVelocity = 0.0f;
	}
	MyBox2d.BOD->SetLinearVelocity(b2Vec2(xVelocity, yVelocity));

	if (PlayerNumber == InputHandler::GetInstance().m_playerInControl) //Lock only one character to move for now
	{
		//Switch active player
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Tab))
		{
			InputHandler::GetInstance().SwitchCharacter(PlayerNumber);
		}

		//Spawn Ball/Grab ball
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::E))
		{
			if (!m_hasThrown && MyBall == nullptr)
			{
				m_hasThrown = true;
				MyBall = new C_Ball(MyBox2d.BOD->GetWorld(), PlayerNumber, Spr_Ball_overlay.getPosition(), b2Vec2(FaceDirection.x, FaceDirection.y));
				m_immuneTimer = 0.0f;
			}
			else if (!m_hasThrown && MyBall != nullptr)
			{
				b2Vec2 direction = MyBox2d.BOD->GetPosition() - MyBall->GetBody()->GetPosition();
				float distance = direction.LengthSquared();
				if (sqrtf(distance) < m_playerGrabRange)
				{
					delete MyBall;
					MyBall = nullptr;
					m_hasThrown = true;
				}
			}
		}
		else if (m_hasThrown)
		{
			m_hasThrown = false;
		}
		float xAxis = 0.0f;
		float yAxis = 0.0f;
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::D))
		{
			xAxis += 1.0f;
		}
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::A))
		{
			xAxis -= 1.0f;
		}

		sf::Vector2i newFacingDirection;
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Left))
		{
			newFacingDirection.x = -1.0f;
		}
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Right))
		{
			newFacingDirection.x = 1.0f;
		}
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Up))
		{
			newFacingDirection.y = 1.0f;
		}
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Down))
		{
			newFacingDirection.y = -1.0f;
		}
		UpdateDirection(newFacingDirection);

		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Space) && m_isGrounded && !m_hasJumped)
		{
			yAxis += 1.0f;
			m_hasJumped = true;
		}
		else if(m_isGrounded)
		{
			m_hasJumped = false;
		}
		//MovePlayer
		b2Vec2 prevVelocity = MyBox2d.BOD->GetLinearVelocity();
		if (yAxis != 0.0f)
		{
			prevVelocity.y += m_playerJumpForce * dt;
		}

		if (xAxis == 0.0f)
		{
			MyBox2d.BOD->SetLinearVelocity(b2Vec2(prevVelocity.x * 0.98f, prevVelocity.y));
		}
		else
		{
			MyBox2d.BOD->SetLinearVelocity(b2Vec2(xAxis * m_playerSpeed * m_playerSpeedMod * dt, prevVelocity.y));
		}

		if (yVelocity > -2.5f)
		{
			MyBox2d.BOD->ApplyForceToCenter(b2Vec2(0, m_playerFallModifier), true);
		}
		//Use Power Up
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Q))
		{
			UsePowerUp();
		}

		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Num1))
		{
			Spr_Emote->setTexture(Tx_Emotes[0]);
			if (m_emoteTimer <= 1.0f)
			{
				m_emoteTimer = 1.0f;
			}
			if (m_emoteTimer <= 0.0f)
				m_emoteTimer = 1.5f;
			return;
		}
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Num2))
		{
			Spr_Emote->setTexture(Tx_Emotes[1]);
			if (m_emoteTimer <= 1.0f)
			{
				m_emoteTimer = 1.0f;
			}
			if (m_emoteTimer <= 0.0f)
				m_emoteTimer = 1.5f;
			return;
		}
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Num3))
		{
			Spr_Emote->setTexture(Tx_Emotes[2]);
			if (m_emoteTimer <= 1.0f)
			{
				m_emoteTimer = 1.0f;
			}
			if (m_emoteTimer <= 0.0f)
				m_emoteTimer = 1.5f;
			return;
		}
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Num4))
		{
			Spr_Emote->setTexture(Tx_Emotes[3]);
			if (m_emoteTimer <= 1.0f)
			{
				m_emoteTimer = 1.0f;
			}
			if (m_emoteTimer <= 0.0f)
				m_emoteTimer = 1.5f;
			return;
		}
	}
}

void C_Player::ProcessImmuneFrames(float dt)
{
	if (m_immuneTimer > 0.0f)
	{
		m_immuneTimer = std::clamp(m_immuneTimer - dt, 0.0f, m_immuneTimer);
		float alpha = std::abs(std::sin(m_immuneTimer * m_immunityFramesSpeed)) * 255;
		Spr_UpperBody.setColor(sf::Color(255, 255, 255, alpha));
		Spr_Legs.setColor(sf::Color(255, 255, 255, alpha));
		sf::Color ballCol = Spr_Ball_overlay.getColor();
		Spr_Ball_overlay.setColor(sf::Color(ballCol.r, ballCol.g, ballCol.b, alpha));
	}
	else
	{
		Spr_UpperBody.setColor(sf::Color(255, 255, 255, 255));
		Spr_Legs.setColor(sf::Color(255, 255, 255, 255));
		sf::Color ballCol = Spr_Ball_overlay.getColor();
		Spr_Ball_overlay.setColor(sf::Color(ballCol.r, ballCol.g, ballCol.b, 255));
	}
}

void C_Player::UpdateDirection(sf::Vector2i newFacingDirection)
{
	FaceDirection = newFacingDirection;
	if (FaceDirection.x > 0.0f)
	{
		if (FaceDirection.y > 0.0f)
		{
			MyDirection = Direction::facing_upRight;
		}
		else if (FaceDirection.y < 0.0f)
		{
			MyDirection = Direction::facing_downRight;
		}
		else
		{
			MyDirection = Direction::facing_right;
		}
	}
	else if (FaceDirection.x < 0.0f)
	{

		if (FaceDirection.y > 0.0f)
		{
			MyDirection = Direction::facing_upLeft;
		}
		else if (FaceDirection.y < 0.0f)
		{
			MyDirection = Direction::facing_downLeft;
		}
		else
		{
			MyDirection = Direction::facing_left;
		}
	}
	else
	{
		if (FaceDirection.y > 0.0f)
		{
			MyDirection = Direction::facing_up;
		}
		else if (FaceDirection.y < 0.0f)
		{
			MyDirection = Direction::facing_down;
		}
		else
		{
			MyDirection = Direction::facing_right;
		}
	}
}

void C_Player::UsePowerUp()
{
	if (myPowerupType == PowerUpType::NONE)
		return;

	switch (myPowerupType)
	{
	case NONE:
		break;
	case SPEED:
		m_playerSpeedMod = 2.0f;
		break;
	case TRIPLESHOT:
		break;
	case SHIELD:
		break;
	case RAILSHOT:
		break;
	case WATERFALL:
		//Sonja's job
		break;
	default:
		break;
	}
	m_powerUpTimer = m_powerUpTimerMax;
	myPowerupType = PowerUpType::NONE;
}

C_Player::~C_Player()
{

}