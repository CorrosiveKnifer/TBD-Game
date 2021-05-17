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

	// player UI Icons and stats
	TX_UI_Player_Icons[0].loadFromFile("Resources/images/UI_Icons/Red_Rudy.png");
	TX_UI_Player_Icons[1].loadFromFile("Resources/images/UI_Icons/Green_Gordon.png");
	TX_UI_Player_Icons[2].loadFromFile("Resources/images/UI_Icons/Blue_Bailey.png");
	TX_UI_Player_Icons[3].loadFromFile("Resources/images/UI_Icons/Yellow_Yara.png");

	SPR_UI_Player_Icons[0].setTexture(TX_UI_Player_Icons[0]);
	SPR_UI_Player_Icons[1].setTexture(TX_UI_Player_Icons[1]);
	SPR_UI_Player_Icons[2].setTexture(TX_UI_Player_Icons[2]);
	SPR_UI_Player_Icons[3].setTexture(TX_UI_Player_Icons[3]);

	SPR_UI_Player_Icons[0].setPosition(0.0f, 0.0f);
	SPR_UI_Player_Icons[1].setPosition(1671.0f, 0.0f);
	SPR_UI_Player_Icons[2].setPosition(0.0f, 801.0f);
	SPR_UI_Player_Icons[3].setPosition(1697.0f, 801.0f);

	myFont.loadFromFile("Resources/fonts/ariblk.ttf");

	Text_UI_Player_Stats[0].setFont(myFont);
	Text_UI_Player_Stats[1].setFont(myFont);
	Text_UI_Player_Stats[2].setFont(myFont);
	Text_UI_Player_Stats[3].setFont(myFont);

	Text_UI_Player_Stats[0].setCharacterSize(24);
	Text_UI_Player_Stats[1].setCharacterSize(24);
	Text_UI_Player_Stats[2].setCharacterSize(24);
	Text_UI_Player_Stats[3].setCharacterSize(24);

	Text_UI_Player_Stats[0].setFillColor(sf::Color::Red);
	Text_UI_Player_Stats[1].setFillColor(sf::Color::Green);
	Text_UI_Player_Stats[2].setFillColor(sf::Color(0,150,255,255));
	Text_UI_Player_Stats[3].setFillColor(sf::Color::Yellow);
	
	Text_UI_Player_Stats[0].setPosition(15.0f, 160.0f);
	Text_UI_Player_Stats[1].setPosition(1717.0f, 160.0f);
	Text_UI_Player_Stats[2].setPosition(15.0f, 960.0f);
	Text_UI_Player_Stats[3].setPosition(1717.0f, 960.0f);
	// Str_UI_Player_Stats[4]; -> in Draw

	// powerup textures for playerCollected powerup UI
	Tx_PowerUps[0].loadFromFile("Resources/images/powerups/pu_NONE.png");
	Tx_PowerUps[1].loadFromFile("Resources/images/powerups/pu_fastRun_2.png");
	Tx_PowerUps[2].loadFromFile("Resources/images/powerups/pu_tripleShot_2.png");
	Tx_PowerUps[3].loadFromFile("Resources/images/powerups/pu_shield_2.png");
	Tx_PowerUps[4].loadFromFile("Resources/images/powerups/pu_railShot_2.png");
	Tx_PowerUps[5].loadFromFile("Resources/images/powerups/pu_waterfall_1.png");
	
	Spr_MyCollectedPowerUp[0].setTexture(Tx_PowerUps[4]);
	Spr_MyCollectedPowerUp[1].setTexture(Tx_PowerUps[4]);
	Spr_MyCollectedPowerUp[2].setTexture(Tx_PowerUps[4]);
	Spr_MyCollectedPowerUp[3].setTexture(Tx_PowerUps[4]);

	Spr_MyCollectedPowerUp[0].setPosition(174.0f, 170.0f);
	Spr_MyCollectedPowerUp[1].setPosition(1863.0f, 170.0f);
	Spr_MyCollectedPowerUp[2].setPosition(174.0f, 970.0f);
	Spr_MyCollectedPowerUp[3].setPosition(1863.0f, 970.0f);
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

	// draw player UI icons and stats
	for (unsigned int i = 0; i < MyPlayers.size(); i++)
	{
		//string tempLives = to_string( MyPlayers[i]->GetLives());

		Str_UI_Player_Stats[i] = "Lives  " + to_string(MyPlayers[i]->GetLives());;
		Str_UI_Player_Stats[i] += "\nScore  " + to_string(MyPlayers[i]->GetScore());
		Text_UI_Player_Stats[i].setString(Str_UI_Player_Stats[i]);
		o_pRenderer->Draw(SPR_UI_Player_Icons[i]);
		o_pRenderer->Draw(Text_UI_Player_Stats[i]);
		o_pRenderer->Draw(Spr_MyCollectedPowerUp[i]);
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

		Spr_MyCollectedPowerUp[it->GetPlayerID() - 1].setTexture(Tx_PowerUps[it->GetPowerUpType()]);
		//if (it->GetPowerUpType() == NONE)
		//{
		//	if (it->GetPlayerID() == 1)
		//	{
		//		Spr_MyCollectedPowerUp[0].setTexture(Tx_PowerUps[4]);
		//	}
		//	if (it->GetPlayerID() == 2)
		//	{
		//		Spr_MyCollectedPowerUp[1].setTexture(Tx_PowerUps[4]);
		//	}
		//	if (it->GetPlayerID() == 3)
		//	{
		//		Spr_MyCollectedPowerUp[2].setTexture(Tx_PowerUps[4]);
		//	}
		//	if (it->GetPlayerID() == 4)
		//	{
		//		Spr_MyCollectedPowerUp[3].setTexture(Tx_PowerUps[4]);
		//	}
		//	continue;
		//}
		//// refactor this code later - below
		//if (it->GetPowerUpType() == SPEED)
		//{
		//	if (it->GetPlayerID() == 1)
		//	{
		//		Spr_MyCollectedPowerUp[0].setTexture(Tx_PowerUps[0]);
		//	}
		//	if (it->GetPlayerID() == 2)
		//	{
		//		Spr_MyCollectedPowerUp[1].setTexture(Tx_PowerUps[0]);
		//	}
		//	if (it->GetPlayerID() == 3)
		//	{
		//		Spr_MyCollectedPowerUp[2].setTexture(Tx_PowerUps[0]);
		//	}
		//	if (it->GetPlayerID() == 4)
		//	{
		//		Spr_MyCollectedPowerUp[3].setTexture(Tx_PowerUps[0]);
		//	}
		//	continue;
		//}
		//if (it->GetPowerUpType() == RAILSHOT)
		//{
		//	if (it->GetPlayerID() == 1)
		//	{
		//		Spr_MyCollectedPowerUp[0].setTexture(Tx_PowerUps[1]);
		//	}
		//	if (it->GetPlayerID() == 2)
		//	{
		//		Spr_MyCollectedPowerUp[1].setTexture(Tx_PowerUps[1]);
		//	}
		//	if (it->GetPlayerID() == 3)
		//	{
		//		Spr_MyCollectedPowerUp[2].setTexture(Tx_PowerUps[1]);
		//	}
		//	if (it->GetPlayerID() == 4)
		//	{
		//		Spr_MyCollectedPowerUp[3].setTexture(Tx_PowerUps[1]);
		//	}
		//	continue;
		//}
		//if (it->GetPowerUpType() == SHIELD)
		//{
		//	if (it->GetPlayerID() == 1)
		//	{
		//		Spr_MyCollectedPowerUp[0].setTexture(Tx_PowerUps[2]);
		//	}
		//	if (it->GetPlayerID() == 2)
		//	{
		//		Spr_MyCollectedPowerUp[1].setTexture(Tx_PowerUps[2]);
		//	}
		//	if (it->GetPlayerID() == 3)
		//	{
		//		Spr_MyCollectedPowerUp[2].setTexture(Tx_PowerUps[2]);
		//	}
		//	if (it->GetPlayerID() == 4)
		//	{
		//		Spr_MyCollectedPowerUp[3].setTexture(Tx_PowerUps[2]);
		//	}
		//	continue;
		//}
		//if (it->GetPowerUpType() == TRIPLESHOT)
		//{
		//	if (it->GetPlayerID() == 1)
		//	{
		//		Spr_MyCollectedPowerUp[0].setTexture(Tx_PowerUps[3]);
		//	}
		//	if (it->GetPlayerID() == 2)
		//	{
		//		Spr_MyCollectedPowerUp[1].setTexture(Tx_PowerUps[3]);
		//	}
		//	if (it->GetPlayerID() == 3)
		//	{
		//		Spr_MyCollectedPowerUp[2].setTexture(Tx_PowerUps[3]);
		//	}
		//	if (it->GetPlayerID() == 4)
		//	{
		//		Spr_MyCollectedPowerUp[3].setTexture(Tx_PowerUps[3]);
		//	}
		//	continue;
		//}
		//if (it->GetPowerUpType() == WATERFALL)
		//{
		//	if (it->GetPlayerID() == 1)
		//	{
		//		Spr_MyCollectedPowerUp[0].setTexture(Tx_PowerUps[5]);
		//	}
		//	if (it->GetPlayerID() == 2)
		//	{
		//		Spr_MyCollectedPowerUp[1].setTexture(Tx_PowerUps[5]);
		//	}
		//	if (it->GetPlayerID() == 3)
		//	{
		//		Spr_MyCollectedPowerUp[2].setTexture(Tx_PowerUps[5]);
		//	}
		//	if (it->GetPlayerID() == 4)
		//	{
		//		Spr_MyCollectedPowerUp[3].setTexture(Tx_PowerUps[5]);
		//	}
		//	continue;
		//}
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
	
	PostUpdate();
}

void c_Level_1::DestroyEntity(Entity* entity)
{
	C_PowerUp* testCase = reinterpret_cast<C_PowerUp*>(entity);
	if (testCase == nullptr)
		return;

	std::vector<C_PowerUp*>::iterator iter = myPowerUps.begin();
	while (iter != myPowerUps.end())
	{
		if ((*iter) == &(*testCase))
		{
			m_toRemove.push_back(entity);
			myPowerUps.erase(iter);
			return;
		}
		iter++;
	}
}

void c_Level_1::PostUpdate()
{
	std::vector<Entity*>::iterator iter = m_toRemove.begin();
	while (iter != m_toRemove.end())
	{
		delete* iter;
		iter = m_toRemove.erase(iter);
	}
	m_toRemove.clear();
}

c_Level_1::~c_Level_1()
{
	
}