#pragma once
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: LevelLoader.h
// Description   	: Level 1, setup everything needed for level 1.
// Author         	: Sonja Fowler
// Mail         	: sonja@alp.co.nz
//
#ifndef _LEVEL_1_
#define _LEVEL_1_


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


class c_Level_1 : public Scene

{
public:
	c_Level_1();
	virtual ~c_Level_1();

	virtual void Draw();
	virtual void Update(float dT);
	virtual void DestroyEntity(Entity* entity);

	void PostUpdate();
	b2World* world = new b2World(C_GlobalVariables::Gravity);


private:
	// collision info
	vector<levelMesh*> myLevel1Meshes;
	std::string path = "Resources/levels/level_1";
	vector<string> dirList;

	// spawn points
	std::string pathSpawn = "Resources/levels/level_1_SpawnInfo";
	vector<b2Vec2> myPlayerSpawnPoints;
	vector<b2Vec2> myPowerUpSpawnPoints;
	b2Vec2 myPowerUpWaterfall;
	
	// Level Graphics
	sf::Texture backgroundTex[3];
	sf::Sprite backgroundSpr[3];
	sf::Font myFont;

	// Players
	vector<C_Player*> MyPlayers;
	// player UI icons
	sf::Texture TX_UI_Player_Icons[4];
	sf::Sprite  SPR_UI_Player_Icons[4];
	sf::Text Text_UI_Player_Stats[4];
	sf::String Str_UI_Player_Stats[4];
	sf::Texture Tx_PowerUps[5];
	sf::Sprite Spr_MyCollectedPowerUp[4];

	// powerups
	vector<C_PowerUp*> myPowerUps;
	vector<Entity*> m_toRemove;
};

#endif // !_LEVEL_1_