#pragma once
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: Level_2.h
// Description   	: Level 2, setup everything needed for level 2.
// Author         	: Sonja Fowler
// Mail         	: sonja@alp.co.nz
//
#ifndef _LEVEL_2_
#define _LEVEL_2_

// system includes
#include <vector>

// Library Includes
#include <Box2D.h>
#include <SFML/Graphics.hpp>

// Parent Include
#include "Scene.h"

// local includes
#include "LevelLoader.h"
#include "Player_Entity.h"
#include "Powerup.h"
#include "SoundBuffer.h"

class c_Level_2 : public Scene

{
public:
	c_Level_2(unsigned int players = 4);
	virtual ~c_Level_2();

	virtual void Draw();
	virtual void Update(float dT);
	virtual void DestroyEntity(Entity* entity);
	void RespawnPlayer(C_Player* player);
	void PostUpdate(float dT);
	b2World* world = new b2World(b2Vec2(0, C_GlobalVariables::Gravity.y));


private:
	// collision info
	vector<levelMesh*> myLevel2Meshes;
	std::string path = "Resources/levels/level_2";
	vector<string> dirList;

	// spawn points
	std::string pathSpawn = "Resources/levels/level_2_SpawnInfo";
	vector<b2Vec2> myPlayerSpawnPoints;
	vector<b2Vec2> myPowerUpSpawnPoints;
	b2Vec2 myPowerUpWaterfall;

	// Level Graphics
	sf::Texture backgroundTex[1];
	sf::Sprite backgroundSpr[1];
	sf::Font myFont;

	// Players
	vector<C_Player*> MyPlayers;

	bool hasWon = false;

	// player UI icons
	sf::Texture TX_UI_Player_Icons[4];
	sf::Sprite  SPR_UI_Player_Icons[4];
	sf::Text Text_UI_Player_Stats[4];
	sf::String Str_UI_Player_Stats[4];
	sf::Texture Tx_PowerUps[6];
	sf::Sprite Spr_MyCollectedPowerUp[4];

	sf::Sprite Spr_Winner;
	sf::Sprite* m_AButton;
	// powerups
	vector<C_PowerUp*> myPowerUps;
	float mf_PowerupTimer = 0.0f;
	float mf_WaterFall_PowerupTimer = 0.0f;
	int mi_Powerup_NewPU = 4;
	int mi_WaterFall_Powerup_NewPU = 24;
	vector<Entity*> m_toRemove;
};

#endif // !_LEVEL_2_