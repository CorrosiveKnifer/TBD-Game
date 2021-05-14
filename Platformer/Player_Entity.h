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

#ifndef _PLAYER_ENTITY_
#define _PLAYER_ENTITY_

// Library Includes
#include <Box2D.h>
#include <SFML/Graphics.hpp>

// parent include
#include "Entity.h"

// local includes
#include "GlobalVariables.h"

class C_Player : Entity
{
public:
	C_Player(b2World* world,int _playerNumber,b2Vec2 _position);
	virtual ~C_Player();

	//Inheritance functions:
	virtual void Draw();
	virtual void Process(float dT);

	//sf::Sprite& GetSpriteLegs() { return Spr_Legs; }
	//sf::Sprite& GetSpr_UpperBody() { return Spr_UpperBody; }
	sf::Sprite& GetSpriteBall() { return Spr_Ball_overlay; }

private: 
	void HandleInput(float dt);
	void UpdateDirection(sf::Vector2i newFacingDirection);
private:
	// textures and sprites 
	sf::Texture Tx_LegsIdle, Tx_LegsJump, Tx_LegsRun[4];
	sf::Texture Tx_UB_ThrowUp1, Tx_UB_ThrowUp2, Tx_UB_ThrowDown1, Tx_UB_ThrowDown2, Tx_UB_ThrowSide1, Tx_UB_ThrowSide2, Tx_UB_ThrowDiagUp1, Tx_UB_ThrowDiagUp2;
	sf::Texture Tx_UB_ThrowDiagDown1, Tx_UB_ThrowDiagDown2, Tx_UB_Shield, Tx_UB_Victory;
	
	sf::Sprite Spr_Legs, Spr_UpperBody;

	// the ball being held - is not a box2d object, simply a temporary overlay
	sf::Texture Tx_MyBall_Overlay;
	sf::Sprite Spr_Ball_overlay;
	sf::Color myBallColor;

	struct body
	{
		b2BodyDef DEF;
		b2PolygonShape SHAPE;
		b2FixtureDef FIX;
		b2Body* BOD;
	};
	body MyBox2d;

	int PlayerNumber; //1,2,3,4  1=Red, 2=Green, 3= Blue,4=Yellow.

	enum Direction{facing_right, facing_upRight, facing_up, facing_upLeft, facing_left, facing_downLeft, facing_down, facing_downRight};
	Direction MyDirection;
	sf::Vector2i FaceDirection;

	enum MovingDirection {moving_none, moving_right, moving_upRight, moving_up, moving_upLeft, moving_left, moving_downLeft, moving_down, moving_downRight };
	MovingDirection MyMovingDirection;
	sf::Vector2i MoveDirection;

	//Player Settings
	float m_playerSpeed = 500;
	float m_playerJumpForce = -800;
	float m_playerFallModifier = 75;

	bool mb_IsInvincible = false;
	bool mb_PlayerHasBall = true;
	bool m_isGrounded = false;
	bool m_hasJumped = false;
	int myHealth = C_GlobalVariables::maxHealth;
	int myLives = C_GlobalVariables::maxLives;

	float mf_Anim_ThrowTime = 0.2f;
	float mf_Anim_ThrowTime_Timer = 0.0f;
	float mf_Anim_RunSpeed = 0.15f;
	float mf_Anim_RunSpeed_Timer = 0.0f;
	int mi_Current_Leg_Frame = 0;
	// add powerups
	// add ball belonging to player (use pointer to this object)
};

#endif