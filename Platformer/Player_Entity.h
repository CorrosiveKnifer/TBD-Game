#pragma once
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: Player_Entity.h
// Description   	: Player Class for any player.
// Author         	: Sonja Fowler
// Mail         	: sonja@alp.co.nz
//
// 
// parent include
#include "Entity.h"

// Library Includes
#include <Box2D.h>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Ball.h"
#include "Powerup.h"
#include "Shield.h"

// local includes
#include "GlobalVariables.h"

enum EmoteType
{
	NO_EMOTE,
	THUMBS_UP,
	LAUGH,
	MAD,
	SHOW_OFF,
};

class C_Player : public Entity
{
public:
	C_Player(b2World* world,int _playerNumber, b2Vec2 _position);
	virtual ~C_Player();

	//Inheritance functions:
	virtual void Draw();
	virtual void Process(float dT);
	void PostUpdate(float dT);
	virtual void HandleHit(Entity* other);

	void Respawn(b2Vec2 position, b2World* world);
	void ApplyPowerUp(PowerUpType type);
	//sf::Sprite& GetSpriteLegs() { return Spr_Legs; }
	//sf::Sprite& GetSpr_UpperBody() { return Spr_UpperBody; }
	b2Vec2 GetPosition() { return MyBox2d.BOD->GetPosition(); };

	sf::Sprite& GetSpriteBall() { return Spr_Ball_overlay; };
	bool IsDead(float& outTimer) { outTimer = m_deathTimer; return m_isDead; };
	virtual bool IsImmune() { return m_immuneTimer > 0; };

	int GetLives() { return this->myLives; };
	int GetScore() { return this->myScore; };
	void SetScore(int _score) { this->myScore += _score; };
	int GetPlayerID() { return PlayerNumber; };

	PowerUpType GetPowerUpType() { return myPowerupType; };

private: 
	void HandleInput(float dt);
	void ProcessImmuneFrames(float dt);
	void UpdateDirection(sf::Vector2i newFacingDirection);
	void UsePowerUp();
	void ThrowBall();
	void Dash(float xAxis);
private:
	b2World* myWorld;
	Shield* myShield;
	PowerUpType myPowerupType;
	bool waterFallTrigger = false;
	PowerUpType myBallPowerUp;
	// textures and sprites 
	
	//Leg Texture references
	sf::Texture* Tx_LegsIdle;
	sf::Texture* Tx_LegsJump;
	sf::Texture* Tx_LegsRun[4];

	sf::Texture* Tx_UB_ThrowUp1; 
	sf::Texture* Tx_UB_ThrowUp2; 
	sf::Texture* Tx_UB_ThrowDown1; 
	sf::Texture* Tx_UB_ThrowDown2; 
	sf::Texture* Tx_UB_ThrowSide1;
	sf::Texture* Tx_UB_ThrowSide2; 
	sf::Texture* Tx_UB_ThrowDiagUp1; 
	sf::Texture* Tx_UB_ThrowDiagUp2;
	sf::Texture* Tx_UB_ThrowDiagDown1; 
	sf::Texture* Tx_UB_ThrowDiagDown2; 
	sf::Texture* Tx_UB_Shield; 
	sf::Texture* Tx_UB_Victory;

	sf::Sprite Spr_Legs, Spr_UpperBody;
	sf::Sprite* Spr_PowerUp;
	sf::Sprite* Spr_Emote;

	sf::Texture* Tx_Emotes[4];

	// the ball being held - is not a box2d object, simply a temporary overlay
	sf::Texture* Tx_MyBall_Overlay;
	sf::Sprite Spr_Ball_overlay;
	sf::Color myBallColor;

	struct body
	{
		b2BodyDef DEF;
		b2PolygonShape SHAPE;
		b2FixtureDef FIX;
		b2Body* BOD;
	};

	C_Ball* MyBall = nullptr;
	std::vector<C_Ball*> MyBall_WaterFall;
	body MyBox2d;

	int PlayerNumber; //1,2,3,4  1=Red, 2=Green, 3= Blue,4=Yellow.
	int controlJoystickID;

	enum Direction{facing_right, facing_upRight, facing_up, facing_upLeft, facing_left, facing_downLeft, facing_down, facing_downRight};
	Direction MyDirection;
	sf::Vector2i FaceDirection;

	enum MovingDirection {moving_none, moving_right, moving_upRight, moving_up, moving_upLeft, moving_left, moving_downLeft, moving_down, moving_downRight };
	MovingDirection MyMovingDirection;
	sf::Vector2i MoveDirection;

	//Player Settings
	float m_playerSpeed = 500;
	float m_playerJumpForce = -400;
	float m_playerFallModifier = 125;
	float m_playerGrabRange = 2.5f;

	bool m_isDead = false;
	float m_deathTimer = 0.0f;
	float m_dashDelay = 0.0f;
	float m_shieldDelay = 0.0f;

	bool mb_IsInvincible = false;
	bool mb_PlayerHasBall = true;
	bool m_isGrounded = false;
	bool m_hasJumped = false;
	bool m_canMoveLeft = true;
	bool m_canMoveRight = true;
	//bool m_hasThrown = false;
	int myHealth = C_GlobalVariables::maxHealth;
	int myLives = C_GlobalVariables::maxLives;
	int myScore = 0;

	float m_pickUpDelay = 0.0f;
	float m_immuneTimer = 0.0f;
	float m_immunityFramesSpeed = 2.0f;
	float m_playerSpeedMod = 1.0f;
	float m_powerUpTimer = 0.0f;
	float m_powerUpTimerMax = 3.0f;

	float mf_Anim_ThrowTime = 0.2f;
	float mf_Anim_ThrowTime_Timer = 0.0f;
	float mf_Anim_RunSpeed = 0.15f;
	float mf_Anim_RunSpeed_Timer = 0.0f;
	int mi_Current_Leg_Frame = 0;
	
	float m_emoteTimer = 0.0f;
	int mi_WaterFall_Count = 0; // 25 balls created for waterfall
	int mi_WaterFall_Count_Orig = 25;
	float mf_WaterFall_Timer = 0.0f;

};