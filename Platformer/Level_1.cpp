// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: LevelLoader.cpp
// Description   	: Level 1, setup everything needed for level 1.
// Author         	: Sonja Fowler
// Mail         	: sonja@alp.co.nz


//parent include
#include "Level_1.h"

// local includes
#include "InputHandler.h"
#include "SceneManager.h"
#include "CollisionListener.h"

c_Level_1::c_Level_1() : Scene()
{
	world->SetContactListener(new CollisionListener());

	// will use spawn point from file for this position info
	MyPlayers.push_back( new C_Player(world, 1,b2Vec2(540.0f,168.0f))); // 1=red player
	MyPlayers.push_back(new C_Player(world, 2, b2Vec2(1383.0f, 168.0f))); // player
	MyPlayers.push_back(new C_Player(world, 3, b2Vec2(405.0f, 660.0f))); // player
	MyPlayers.push_back(new C_Player(world, 4, b2Vec2(1521.0f, 660.0f))); // player

	// level load for colliders from 3d .obj file
	for (const auto& entry : fs::directory_iterator(path))
	{
		// std::cout << entry.path() << std::endl;

		std::string fName = entry.path().string();
		cout << fName << endl;
		dirList.push_back(fName);
	}

	// load all directory (level 1) meshes
	for (auto it : dirList)
	{
		myLevel1Meshes.push_back(new levelMesh(it, *world));
	}

	// Level 1 textures
	backgroundTex[0].loadFromFile("Resources/images/level_1/Level_1_01a_Back.png");
	backgroundTex[1].loadFromFile("Resources/images/level_1/Level_1_01a_middle.png");
	backgroundTex[2].loadFromFile("Resources/images/level_1/Level_1_01a_Top.png");

	backgroundSpr[0].setTexture(backgroundTex[0]);
	backgroundSpr[1].setTexture(backgroundTex[1]);
	backgroundSpr[2].setTexture(backgroundTex[2]);


	// load in spawn points for players and powerups

	for (const auto& entry : fs::directory_iterator(pathSpawn))
	{
		std::string fName = entry.path().string();
		cout << fName << endl;
		levelMesh::levelSpawnPoints(fName, myPlayerSpawnPoints , myPowerUpSpawnPoints, myPowerUpWaterfall);
		
	}
	// update for how many players playing ---------------------------------------<<<<<<<
	MyPlayers.push_back(new C_Player(world, 1, myPlayerSpawnPoints[0])); // player
	MyPlayers.push_back(new C_Player(world, 2, myPlayerSpawnPoints[1])); // player
	MyPlayers.push_back(new C_Player(world, 3, myPlayerSpawnPoints[2])); // player
	MyPlayers.push_back(new C_Player(world, 4, myPlayerSpawnPoints[3])); // player

	// create powerups
	for (unsigned int i = 0; i < 4; i++)
	{
		myPowerUps.push_back(new C_PowerUp(world, myPowerUpSpawnPoints[i],i+1));
	}
	myPowerUps.push_back(new C_PowerUp(world, myPowerUpWaterfall, 5));
	for (unsigned int i = 4; i < myPowerUpSpawnPoints.size(); i++)
	{
		myPowerUps.push_back(new C_PowerUp(world, myPowerUpSpawnPoints[i], 4));
	}
}


void c_Level_1::Draw()
{
	o_pRenderer->Draw(backgroundSpr[0]);
	o_pRenderer->Draw(backgroundSpr[1]);
	o_pRenderer->Draw(backgroundSpr[2]);

	for (auto it : MyPlayers)
	{
		it->Draw();
	}
	for (auto it : myPowerUps)
	{
		it->Draw();
	}
	
}

void c_Level_1::Update(float dT)
{
	// update world
	(*world).Step(C_GlobalVariables::timeStep, 8, 3);

	//update all players
	for (auto it : MyPlayers)
	{
		it->Process(dT);
	}
	for (auto it : MyPlayers)
	{
		float timer;
		if (it->IsDead(timer))
		{
			if (timer >= 3.0f)
			{
				it->Respawn(b2Vec2(10, 10), world);
			}
		}
	}
	for (auto it : myPowerUps)
	{
		it->Process(dT);
	}
}

c_Level_1::~c_Level_1()
{
	
}