// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: Player_Entity.cpp
// Description   	: Player Class for any player.
// Author         	: Sonja Fowler, Michael Jordan, Bailey Johnson 
// Mail         	: sonja@alp.co.nz
//

#include "Player_Entity.h"
#include "SceneManager.h"
#include "RayCastClass.h"
#include "SoundBuffer.h"

#include <iostream>

C_Player::C_Player(b2World* world,int _playerNumber, b2Vec2 _position) : Entity()
{
	PlayerNumber = _playerNumber;
	controlJoystickID = InputHandler::GetInstance().playerJoystickIDs.at(_playerNumber - 1);

	std::string tempPath = "";
	//1,2,3,4  1=Red, 2=Green, 3= Blue,4=Yellow.
	if (PlayerNumber == 1)
	{
		tempPath = "images/Characters/Finals_Red_Sized/";
		myBallColor = sf::Color::Red;
		myScore = C_GlobalVariables::Player_1_Score;
	}
	if (PlayerNumber == 2)
	{
		tempPath = "images/Characters/Finals_GREEN_Sized/";
		myBallColor = sf::Color::Green;
		myScore = C_GlobalVariables::Player_2_Score;
	}
	if (PlayerNumber == 3)
	{
		tempPath = "images/Characters/Finals_BLUE_Sized/";
		myBallColor = sf::Color(0, 150, 255, 255);
		myScore = C_GlobalVariables::Player_3_Score;
	}
	if (PlayerNumber == 4)
	{
		tempPath = "images/Characters/Finals_YELLOW_sized/";
		myBallColor = sf::Color::Yellow;
		myScore = C_GlobalVariables::Player_4_Score;
	}

	//Renderer can do this for you.
	Tx_LegsIdle = Renderer::GetInstance().CreateTexture(tempPath + "_0013_Legs_Idle.png");
	Tx_LegsJump = Renderer::GetInstance().CreateTexture(tempPath + "_0012_Legs_Jump.png");
	
	Tx_LegsRun[0] = Renderer::GetInstance().CreateTexture(tempPath + "_0009_Legs_Run_1.png");
	Tx_LegsRun[1] = Renderer::GetInstance().CreateTexture(tempPath + "_0011_Legs_Run_2.png");
	Tx_LegsRun[2] = Renderer::GetInstance().CreateTexture(tempPath + "_0010_Legs_Run_3.png");
	Tx_LegsRun[3] = Renderer::GetInstance().CreateTexture(tempPath + "_0011_Legs_Run_4.png");

	Tx_UB_ThrowUp1 = Renderer::GetInstance().CreateTexture(tempPath + "_0001_Throw_Up_1.png");
	Tx_UB_ThrowUp2 = Renderer::GetInstance().CreateTexture(tempPath + "_0000_Throw_Up_2.png");
	Tx_UB_ThrowDown1 = Renderer::GetInstance().CreateTexture(tempPath + "_0003_Throw_Down_1.png");
	Tx_UB_ThrowDown2 = Renderer::GetInstance().CreateTexture(tempPath + "_0002_Throw_Down_2.png");
	Tx_UB_ThrowSide1 = Renderer::GetInstance().CreateTexture(tempPath + "_0016_Throw_Mid_1.png");
	Tx_UB_ThrowSide2 = Renderer::GetInstance().CreateTexture(tempPath + "_0015_Throw_Mid_2.png");
	Tx_UB_ThrowDiagUp1 = Renderer::GetInstance().CreateTexture(tempPath + "_0005_Throw_DiagUp_1.png");
	Tx_UB_ThrowDiagUp2 = Renderer::GetInstance().CreateTexture(tempPath + "_0004_Throw_DiagUp_2.png");

	Tx_UB_ThrowDiagDown1 = Renderer::GetInstance().CreateTexture(tempPath + "_0007_Throw_DiagDown_1.png");
	Tx_UB_ThrowDiagDown2 = Renderer::GetInstance().CreateTexture(tempPath + "_0006_Throw_DiagDown_2.png");
	Tx_UB_Shield = Renderer::GetInstance().CreateTexture(tempPath + "_0008_Shield.png");
	Tx_UB_Victory = Renderer::GetInstance().CreateTexture(tempPath + "_0014_Victory.png");

	Tx_MyBall_Overlay = Renderer::GetInstance().CreateTexture("images/Ball/Ball.png");

	// sprite setup
	Spr_Legs.setTexture(*Tx_LegsIdle);
	Spr_UpperBody.setTexture(*Tx_UB_ThrowSide1);
	Spr_Legs.setOrigin(41.0f, 82.0f);
	Spr_UpperBody.setOrigin(41.0f, 82.0f);

	Spr_Ball_overlay.setTexture(*Tx_MyBall_Overlay);
	Spr_Ball_overlay.setOrigin(Tx_MyBall_Overlay->getSize().x / 2.0f, Tx_MyBall_Overlay->getSize().y / 2.0f);
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
	Tx_Emotes[0] = Renderer::GetInstance().CreateTexture("images/Emotes/Emote1.png");
	Tx_Emotes[1] = Renderer::GetInstance().CreateTexture("images/Emotes/Emote2.png");
	Tx_Emotes[2] = Renderer::GetInstance().CreateTexture("images/Emotes/Emote3.png");
	Tx_Emotes[3] = Renderer::GetInstance().CreateTexture("images/Emotes/Emote4.png");

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

		if (Spr_PowerUp != nullptr)
			Renderer::GetInstance().Draw(*Spr_PowerUp);
	}

	if (MyBall_WaterFall.size() > 0)
	{
		for (auto it : MyBall_WaterFall)
		{
			it->Draw();
		}
	}
	if (MyBall_WaterFall.size() > 0)
	{
		for (unsigned int i = 0; i < MyBall_WaterFall.size();i++)
		{
			if (MyBall_WaterFall[i]->GetBounceCount() <= 0)
			{
				delete MyBall_WaterFall[i];
				MyBall_WaterFall[i] = nullptr;
			}
		}
	}
	for (unsigned int i = 0; i < MyBall_WaterFall.size();)
	{
		if (MyBall_WaterFall[i] == nullptr)
		{
			MyBall_WaterFall.erase(MyBall_WaterFall.begin()+i);
		}
		else
		{
			i++;
		}
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

	if (myShield != nullptr)
	{
		myShield->Draw();
	}
}


// ------------------------------------------------- Process -----------------------------------------------------------------
void C_Player::Process(float dT)
{
	if (myShield != nullptr)
	{
		m_shieldDelay -= dT;
		if (m_shieldDelay <= 0)
		{
			delete myShield;
			myShield = 0;
		}
		else
		{
			myShield->SetPosition(MyBox2d.BOD->GetPosition());
			myShield->Process(dT);
		}
	}
	if (m_dashDelay > 0)
	{
		m_dashDelay -= dT;
	}

	// process waterfall effect
	if (MyBall_WaterFall.size() > 0)
	{
		for (auto it : MyBall_WaterFall)
		{
			// update sprite pos.
			it->Process(dT);
		}
	}


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

	if (m_playerSpeedMod > 1.0f)
	{
		m_playerSpeedMod = 1.0f + 1.0f * (m_powerUpTimer / m_powerUpTimerMax);
	}

	if(MyBall != nullptr)
		MyBall->Process(dT);

	// Make the WATERFALL!!!
	if (myPowerupType == WATERFALL && mi_WaterFall_Count != 0)
	{
		mf_WaterFall_Timer += dT;
		if (mf_WaterFall_Timer > 0.2f)
		{
			mf_WaterFall_Timer = 0.0f;
			//Note: Water fall count converges to 0 balls remaining. Negative represents the other hole.
			if (mi_WaterFall_Count > 0)
			{
				//Left side
				MyBall_WaterFall.push_back(new C_Ball(MyBox2d.BOD->GetWorld(), PlayerNumber, sf::Vector2f(790.0f, 5.0f), b2Vec2(-0.5f - ((float)(std::abs(mi_WaterFall_Count)) / 100.0f), -0.5f), true));
				mi_WaterFall_Count--;
			}
			else if (mi_WaterFall_Count < 0)
			{
				//Right side
				MyBall_WaterFall.push_back(new C_Ball(MyBox2d.BOD->GetWorld(), PlayerNumber, sf::Vector2f(1200.0f, 5.0f), b2Vec2(-0.5f - ((float)(std::abs(mi_WaterFall_Count)) / 100.0f), -0.5f), true));
				mi_WaterFall_Count++;
			}
			
		}
	}
	if (myPowerupType == WATERFALL && mi_WaterFall_Count == 0)
	{
		myPowerupType = NONE;
		mi_WaterFall_Count = 0;
	}

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
	MyBox2d.BOD->GetWorld()->RayCast(&RayResult, MyBox2d.BOD->GetPosition(), MyBox2d.BOD->GetPosition() + b2Vec2(-1 * MoveDirection.x, 2));
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

	if (this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM > C_GlobalVariables::ScreenSizeX)
	{
		this->MyBox2d.BOD->SetTransform(b2Vec2(0, this->MyBox2d.BOD->GetPosition().y), 0);
	}
	else if (this->MyBox2d.BOD->GetPosition().x * C_GlobalVariables::PPM < 0)
	{
		this->MyBox2d.BOD->SetTransform(b2Vec2(C_GlobalVariables::ScreenSizeX / C_GlobalVariables::PPM, this->MyBox2d.BOD->GetPosition().y), 0);
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
		this->Spr_Legs.setTexture(*Tx_LegsRun[mi_Current_Leg_Frame]);
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
		this->Spr_Legs.setTexture(*Tx_LegsRun[mi_Current_Leg_Frame]);
		this->Spr_Legs.setScale(1.0f, 1.0f);
		
	}

	// stationary = Idle legs.
	if (this->MyBox2d.BOD->GetLinearVelocity().x > -C_GlobalVariables::minimumSpeedForAnim && this->MyBox2d.BOD->GetLinearVelocity().x < C_GlobalVariables::minimumSpeedForAnim)
	{
		if (MyDirection == C_Player::facing_right || MyDirection == C_Player::facing_upRight || MyDirection == C_Player::facing_downRight || MyDirection == C_Player::facing_down || MyDirection == C_Player::facing_up)
		{
			this->Spr_Legs.setTexture(*Tx_LegsIdle);
			this->Spr_Legs.setScale(1.0f, 1.0f);
		}
		else
		{
			this->Spr_Legs.setTexture(*Tx_LegsIdle);
			this->Spr_Legs.setScale(-1.0f, 1.0f);
		}
		
		//reset
		mi_Current_Leg_Frame = 0;
		mf_Anim_RunSpeed_Timer = 0.0f;
	}

	// final -  override for legs if we are airborne
	if (this->MyBox2d.BOD->GetLinearVelocity().y > C_GlobalVariables::minimumSpeedForAnimJump || this->MyBox2d.BOD->GetLinearVelocity().y < -C_GlobalVariables::minimumSpeedForAnimJump)
	{
		this->Spr_Legs.setTexture(*Tx_LegsJump);
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
		this->Spr_UpperBody.setTexture(*Tx_UB_ThrowSide1);
		break;
	case C_Player::facing_upRight:
		this->Spr_UpperBody.setTexture(*Tx_UB_ThrowDiagUp1);
		break;
	case C_Player::facing_up:
		this->Spr_UpperBody.setTexture(*Tx_UB_ThrowUp1);
		break;
	case C_Player::facing_upLeft:
		this->Spr_UpperBody.setTexture(*Tx_UB_ThrowDiagUp1);
		break;
	case C_Player::facing_left:
		this->Spr_UpperBody.setTexture(*Tx_UB_ThrowSide1);
		break;
	case C_Player::facing_downLeft:
		this->Spr_UpperBody.setTexture(*Tx_UB_ThrowDiagDown1);
		break;
	case C_Player::facing_down:
		this->Spr_UpperBody.setTexture(*Tx_UB_ThrowDown1);
		break;
	case C_Player::facing_downRight:
		this->Spr_UpperBody.setTexture(*Tx_UB_ThrowDiagDown1);
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
		if (Spr_PowerUp != nullptr)
		{
			Spr_PowerUp->setPosition(Spr_Ball_overlay.getPosition());
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
		SoundBuffer::GetInstance().PlaySoundEffect("big_buzz");
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
	if (type == PowerUpType::WATERFALL)
	{
		int direct = (rand() % 2); //Decide which direction the waterfall is coming from.
		if (direct == 1)
		{
			mi_WaterFall_Count = mi_WaterFall_Count_Orig;
		}
		else
		{
			mi_WaterFall_Count = -1.0f * mi_WaterFall_Count_Orig;
		}
		SoundBuffer::GetInstance().PlaySoundEffect("big_dong");
	}
	else if (type != PowerUpType::NONE)
	{
		SoundBuffer::GetInstance().PlaySoundEffect("powerup_collect1");
	}

	myPowerupType = type;
}

void C_Player::HandleInput(float dt)
{
	if (m_pickUpDelay > 0)
	{
		m_pickUpDelay -= dt;
	}

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

	//if (PlayerNumber == InputHandler::GetInstance().m_playerInControl) //Lock only one character to move for now
	//{
		//Controller Switch Player
		//if (sf::Joystick::isButtonPressed(controlJoystickID, InputHandler::GetInstance().BUTTON_BACK))
		//{
		//	InputHandler::GetInstance().SwitchCharacter(PlayerNumber);
		//}

		////Switch active player
		//if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::Tab))
		//{
		//	InputHandler::GetInstance().SwitchCharacter(PlayerNumber);
		//}

		//Controller Shoot input
		if (sf::Joystick::isButtonPressed(controlJoystickID, InputHandler::GetInstance().BUTTON_RB))
		{
			if (m_pickUpDelay <= 0.0f && MyBall == nullptr) //Throw
			{
				ThrowBall();
				m_pickUpDelay = 0.4f;
			}
			else if (m_pickUpDelay <= 0.0f) //Pick up
			{
				b2Vec2 direction = MyBox2d.BOD->GetPosition() - MyBall->GetBody()->GetPosition();
				float distance = direction.LengthSquared();
				if (sqrtf(distance) < m_playerGrabRange)
				{
					delete MyBall;
					MyBall = nullptr;
					m_pickUpDelay = 0.4f;
				}
			}
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
		
		//Controller Movement
		if (InputHandler::GetInstance().GetMovementInput(controlJoystickID).x != 0)
		{
			//Left
			if (InputHandler::GetInstance().GetMovementInput(controlJoystickID).x <= -20)
			{
				xAxis -= 1.0f;
				MoveDirection.x = -1.0f;
			}
			//Right
			if (InputHandler::GetInstance().GetMovementInput(controlJoystickID).x >= 20)
			{
				xAxis += 1.0f;
				MoveDirection.x = 1.0f;
			}
		}

		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::D))
		{
			xAxis += 1.0f;
		}
		if (InputHandler::GetInstance().IsKeyPressed(sf::Keyboard::A))
		{
			xAxis -= 1.0f;
		}
		sf::Vector2i newFacingDirection;

		//Controller Aiming
		if (InputHandler::GetInstance().GetAimInput(controlJoystickID).x <= -20 || 
			InputHandler::GetInstance().GetAimInput(controlJoystickID).x >= 20 || 
			InputHandler::GetInstance().GetAimInput(controlJoystickID).y <= -20 ||
			InputHandler::GetInstance().GetAimInput(controlJoystickID).y >= 20)
		{
			//Left
			if (InputHandler::GetInstance().GetAimInput(controlJoystickID).x <= -20)
			{
				newFacingDirection.x = -1.0f;
			}
			//Right
			if (InputHandler::GetInstance().GetAimInput(controlJoystickID).x >= 20)
			{
				newFacingDirection.x = 1.0f;
			}
			//Up
			if (InputHandler::GetInstance().GetAimInput(controlJoystickID).y <= -20)
			{
				newFacingDirection.y = 1.0f;
			}
			//Down
			if (InputHandler::GetInstance().GetAimInput(controlJoystickID).y >= 20)
			{
				newFacingDirection.y = -1.0f;
			}
			UpdateDirection(newFacingDirection);
		}

		//Controller Jump
		if (sf::Joystick::isButtonPressed(controlJoystickID, InputHandler::GetInstance().BUTTON_A) && m_isGrounded && !m_hasJumped || sf::Joystick::isButtonPressed(controlJoystickID, InputHandler::GetInstance().BUTTON_LB) && m_isGrounded && !m_hasJumped)
		{
			yAxis += 1.0f;
			m_hasJumped = true;
		}
		else if (m_isGrounded)
		{
			m_hasJumped = false;
		}

		//Controller Dodge
		if (sf::Joystick::isButtonPressed(controlJoystickID, InputHandler::GetInstance().BUTTON_B))
		{
			Dash(MoveDirection.x);
		}

		//Controller Pause
		if (sf::Joystick::isButtonPressed(controlJoystickID, InputHandler::GetInstance().BUTTON_START))
		{
			SceneManager::GetInstance().PauseScene();
		}

		//Controller Show Ball (Radar)
		if (sf::Joystick::isButtonPressed(controlJoystickID, InputHandler::GetInstance().BUTTON_X))
		{
			//Show Ball

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

		//Controller Use PowerUp
		if (sf::Joystick::isButtonPressed(controlJoystickID, InputHandler::GetInstance().BUTTON_Y))
		{
			UsePowerUp();
		}

		//Controller Emotes
		if (InputHandler::GetInstance().GetEmoteInput(controlJoystickID).x != 0 || InputHandler::GetInstance().GetEmoteInput(controlJoystickID).y != 0)
		{
			//Dpad Up
			if (InputHandler::InputHandler::GetInstance().GetEmoteInput(controlJoystickID).y >= 20)
			{
				Spr_Emote->setTexture(*Tx_Emotes[0]);
				if (m_emoteTimer <= 1.0f)
				{
					m_emoteTimer = 1.0f;
				}
				if (m_emoteTimer <= 0.0f)
					m_emoteTimer = 1.5f;
				return;
			}
			//Dpad Left
			if (InputHandler::InputHandler::GetInstance().GetEmoteInput(controlJoystickID).x <= 20)
			{
				Spr_Emote->setTexture(*Tx_Emotes[1]);
				if (m_emoteTimer <= 1.0f)
				{
					m_emoteTimer = 1.0f;
				}
				if (m_emoteTimer <= 0.0f)
					m_emoteTimer = 1.5f;
				return;
			}
			//Dpad Down
			if (InputHandler::InputHandler::GetInstance().GetEmoteInput(controlJoystickID).y <= 20)
			{
				Spr_Emote->setTexture(*Tx_Emotes[2]);
				if (m_emoteTimer <= 1.0f)
				{
					m_emoteTimer = 1.0f;
				}
				if (m_emoteTimer <= 0.0f)
					m_emoteTimer = 1.5f;
				return;
			}
			//Dpad Right
			if (InputHandler::InputHandler::GetInstance().GetEmoteInput(controlJoystickID).x >= 20)
			{
				Spr_Emote->setTexture(*Tx_Emotes[3], true);
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

	// play use sound
	SoundBuffer::GetInstance().PlaySoundEffect("powerup");

	switch (myPowerupType)
	{
	case NONE:
		break;
	case SPEED:
		m_playerSpeedMod = 2.0f;
		break;
	case TRIPLESHOT:
		if (MyBall != nullptr)
			return;

		if (Spr_PowerUp == nullptr)
		{
			Spr_PowerUp = new sf::Sprite();
			Spr_PowerUp->setOrigin(sf::Vector2f(25, 25));
		}
		Spr_PowerUp->setTexture(*Renderer::GetInstance().CreateTexture("images/powerups/pu_tripleShot_1.png"), true);
		myBallPowerUp = myPowerupType;
		break;
	case SHIELD:
		if(myShield == nullptr)
			myShield = new Shield(MyBox2d.BOD->GetWorld(), PlayerNumber, MyBox2d.BOD->GetPosition());
		m_shieldDelay = 3.0f;
		break;
	case RAILSHOT:
		if (MyBall != nullptr)
			return;

		if (Spr_PowerUp == nullptr)
		{
			Spr_PowerUp = new sf::Sprite();
			Spr_PowerUp->setOrigin(sf::Vector2f(25, 25));
		}
		myBallPowerUp = myPowerupType;
		Spr_PowerUp->setTexture(*Renderer::GetInstance().CreateTexture("images/powerups/pu_railShot_1.png"), true);
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

void C_Player::ThrowBall()
{
	switch (myBallPowerUp)
	{
	case TRIPLESHOT:
	{
		MyBall = new C_Ball(MyBox2d.BOD->GetWorld(), PlayerNumber, Spr_Ball_overlay.getPosition(), b2Vec2(FaceDirection.x, FaceDirection.y));

		C_Ball* temp = new C_Ball(MyBox2d.BOD->GetWorld(), PlayerNumber, Spr_Ball_overlay.getPosition(), b2Vec2(FaceDirection.y * 0.2, -FaceDirection.x * 0.2) + b2Vec2(FaceDirection.x * 0.8, FaceDirection.y * 0.8), true);
		MyBall_WaterFall.push_back(temp);
		temp = new C_Ball(MyBox2d.BOD->GetWorld(), PlayerNumber, Spr_Ball_overlay.getPosition(), b2Vec2(-FaceDirection.y * 0.2, FaceDirection.x * 0.2) + b2Vec2(FaceDirection.x * 0.8, FaceDirection.y * 0.8), true);
		MyBall_WaterFall.push_back(temp);
		delete Spr_PowerUp;
		Spr_PowerUp = nullptr;
		break;
	}
	case RAILSHOT:
		MyBall = new C_Ball(MyBox2d.BOD->GetWorld(), PlayerNumber, Spr_Ball_overlay.getPosition(), b2Vec2(FaceDirection.x, FaceDirection.y));
		MyBall->AddForce(b2Vec2(FaceDirection.x * 50.0f, FaceDirection.y * 50.0f));
		MyBall->SetBounceCount(10);
		delete Spr_PowerUp;
		Spr_PowerUp = nullptr;
		break;
	default:
		MyBall = new C_Ball(MyBox2d.BOD->GetWorld(), PlayerNumber, Spr_Ball_overlay.getPosition(), b2Vec2(FaceDirection.x, FaceDirection.y));
		break;
	}
	myBallPowerUp = NONE;

	//m_hasThrown = true;
	m_immuneTimer = 0.0f;
}

void C_Player::Dash(float xAxis)
{
	if(m_dashDelay <= 0 && m_immuneTimer <= 0.0f)
	{
		MyBox2d.BOD->ApplyForceToCenter(b2Vec2(10000.0f * xAxis, -200), true);
		m_dashDelay = 1.5f;
		m_immuneTimer = 0.16f;
		SoundBuffer::GetInstance().PlaySoundEffect("Dash");
	}
}

C_Player::~C_Player()
{
	if (myShield != nullptr)
	{
		delete myShield;
		myShield = nullptr;
	}

	delete Spr_PowerUp;
	Spr_PowerUp = nullptr;

	delete Spr_Emote;
	Spr_Emote = nullptr;

	MyBox2d.BOD->GetWorld()->DestroyBody(MyBox2d.BOD);

	if (MyBall != nullptr)
	{
		delete MyBall;
		MyBall = nullptr;
	}

	std::vector<C_Ball*>::iterator iter = MyBall_WaterFall.begin();
	while (iter != MyBall_WaterFall.end())
	{
		delete * iter;
		iter = MyBall_WaterFall.erase(iter);
	}
}