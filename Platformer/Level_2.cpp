// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2021 Media Design School
//
// File Name     	: Level_2.cpp
// Description   	: Level 2, setup everything needed for level 2.
// Author         	: Sonja Fowler
// Mail         	: sonja@alp.co.nz


//parent include
#include "Level_2.h"

// local includes
#include "InputHandler.h"
#include "SceneManager.h"
#include "CollisionListener.h"
#include "VictoryScene.h"

// c_Level_2(unsigned int players) : Scene()
//
// Description:	Creates a game level to play in, loads all graphics, collisions information, spawn points,creates players,
//				powerups, the HUD graphics and text for player information
//				
// @param	unsigned int	The amount of players to spawn in this level
//
// Inheritance from: Scene()
// 
// @return	c_Level_2 object
//
c_Level_2::c_Level_2(unsigned int players) : Scene()
{
	sceneID = LEVEL2;
	C_GlobalVariables::CurrentLevel = 2;

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
		myLevel2Meshes.push_back(new levelMesh(it, *world));
	}

	// Level 1 textures
	backgroundTex[0].loadFromFile("Resources/images/level_2/Level_2_all.png");


	backgroundSpr[0].setTexture(backgroundTex[0]);



	// load in spawn points for players and powerups

	for (const auto& entry : fs::directory_iterator(pathSpawn))
	{
		std::string fName = entry.path().string();
		cout << fName << endl;
		levelMesh::levelSpawnPoints(fName, myPlayerSpawnPoints, myPowerUpSpawnPoints, myPowerUpWaterfall);

	}

	// update for how many players playing ---------------------------------------<<<<<<<
	for (unsigned int i = 0; i < players; i++)
	{
		MyPlayers.push_back(new C_Player(world, i + 1, myPlayerSpawnPoints[i])); // player
	}

	srand(time(0));
	// create powerups
	/*for (unsigned int i = 0; i < 4; i++)
	{
		myPowerUps.push_back(new C_PowerUp(world, myPowerUpSpawnPoints[i],i+1));
	}*/
	//myPowerUps.push_back(new C_PowerUp(world, myPowerUpWaterfall, 5));
	/*for (unsigned int i = 4; i < myPowerUpSpawnPoints.size(); i++)
	{
		myPowerUps.push_back(new C_PowerUp(world, myPowerUpSpawnPoints[i], 4));
	}*/

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
	Text_UI_Player_Stats[2].setFillColor(sf::Color(0, 150, 255, 255));
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

	Spr_Winner.setPosition(C_GlobalVariables::ScreenSizeX / 2, C_GlobalVariables::ScreenSizeY * 0.40);
	m_AButton = Renderer::GetInstance().CreateSprite("images/UI_Icons/A button.png");
	m_AButton->setScale(0.25, 0.25);
	m_AButton->setOrigin(128, 128);
}

// Draw()
//
// Description:	draw the level graphics and then all of the visible objects active in the level
//				
// @param	
//
// 
// @return	NA
//
void c_Level_2::Draw()
{
	o_pRenderer->Draw(backgroundSpr[0]);

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

	if (hasWon)
	{
		Spr_Winner.setScale(sf::Vector2f(2, 2));
		o_pRenderer->Draw(Spr_Winner);
		o_pRenderer->SetFontSize(25);
		o_pRenderer->DrawAt(*m_AButton, sf::Vector2f(C_GlobalVariables::ScreenSizeX / 2, C_GlobalVariables::ScreenSizeY * 0.60));
	}
}

// Update(float dT)
//
// Description:	Updates the level : all the players, the powerups, the balls, the players HUD information
//				
// @param	float		delta time
//
// 
// @return	NA
//
void c_Level_2::Update(float dT)
{
	// update world
	(*world).Step(C_GlobalVariables::timeStep, 8, 3);

	//update all players
	for (auto it : MyPlayers)
	{
		it->Process(dT);

		Spr_MyCollectedPowerUp[it->GetPlayerID() - 1].setTexture(Tx_PowerUps[it->GetPowerUpType()]);
	}
	unsigned int playersRemaining = 0;
	for (auto it : MyPlayers)
	{
		float timer;
		if (it->IsDead(timer) && it->GetLives() > 0)
		{
			if (timer >= 3.0f)
			{
				RespawnPlayer(it);
			}
		}

		if (it->GetLives() > 0)
		{
			playersRemaining++;
		}
	}
	if (playersRemaining <= 1)
	{
		//SOMEONE HAS WON
		if (!hasWon)
		{
			for (int i = 0; i < MyPlayers.size(); i++)
			{
				if (MyPlayers.at(i)->GetLives() > 0)
				{
					Spr_Winner.setOrigin(TX_UI_Player_Icons[i].getSize().x / 2, TX_UI_Player_Icons[i].getSize().y / 2);
					Spr_Winner.setTexture(TX_UI_Player_Icons[i]);
					hasWon = true;

				}

			}
		}
		else
		{
			for (int i = 0; i < 4; i++)
			{
				if (sf::Joystick::isButtonPressed(i, InputHandler::GetInstance().BUTTON_A))
				{
					C_GlobalVariables::Player_1_Score = MyPlayers[0]->GetScore();

					if (MyPlayers.size() > 1)
					{
						C_GlobalVariables::Player_2_Score = MyPlayers[1]->GetScore();
					}
					if (MyPlayers.size() > 2)
					{
						C_GlobalVariables::Player_3_Score = MyPlayers[2]->GetScore();
					}
					if (MyPlayers.size() > 3)
					{
						C_GlobalVariables::Player_4_Score = MyPlayers[3]->GetScore();
					}

					SceneManager::GetInstance().TransitionTo(new VictoryScene());
				}
			}
		}
	}

	for (auto it : myPowerUps)
	{
		it->Process(dT);
	}


	// Powerups update
	mf_PowerupTimer += dT;
	mf_WaterFall_PowerupTimer += dT;

	if (mf_PowerupTimer > mi_Powerup_NewPU) // time to spawn a powerup?
	{
		mf_PowerupTimer = 0.0f; // reset timer

		int tempCheck = 0;
		for (auto it : myPowerUps) // check if the waterfall PU is in game
		{
			if (it->myPowerupType == 5)
			{
				tempCheck++;
			}
		}
		if (tempCheck < 1 && mf_WaterFall_PowerupTimer > mi_WaterFall_Powerup_NewPU) // time for a waterfall PU.
		{
			mf_WaterFall_PowerupTimer = 0.0f; // reset WF timer

			myPowerUps.push_back(new C_PowerUp(world, myPowerUpWaterfall, 5, 8));
		}
		if (tempCheck > 0) { mf_WaterFall_PowerupTimer = 0.0f; } // reset WF_PU timer


		if (myPowerUps.size() < 4 ) // time for another powerup
		{
			int tempPos = rand() % myPowerUpSpawnPoints.size();

			if (C_PowerUp::positionIsReserved[tempPos] == true)
			{
				for (unsigned int i = 0; i < myPowerUpSpawnPoints.size(); i++)
				{
					if (C_PowerUp::positionIsReserved[i] == false)
					{
						tempPos = i;
						C_PowerUp::positionIsReserved[i] = true;
						break;
					}
				}
			}
			int tempType = rand() % 4 + 1;

			myPowerUps.push_back(new C_PowerUp(world, myPowerUpSpawnPoints[tempPos], tempType, tempPos));
		}


	}

	PostUpdate(dT);
}

// DestroyEntity(Entity* entity)
//
// Description:	Destroys an entity, placing it in a list of objects to be destroyed at the opportune moment.
//				
// @param	Entity*		the entity to destroy.
//
// 
// @return	NA
//
void c_Level_2::DestroyEntity(Entity* entity)
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

// RespawnPlayer(C_Player* player)
//
// Description:	Respawns a player to a position worked out to be safest in the world.
//				
// @param	C_Player*	The player to re-spawn
//
// 
// @return	NA
//
void c_Level_2::RespawnPlayer(C_Player* player)
{
	b2Vec2 averagePos(0, 0);
	unsigned int playerCount = 0;

	for (C_Player* currPlayer : MyPlayers)
	{
		float time;
		if (!currPlayer->IsDead(time))
		{
			averagePos += currPlayer->GetPosition();
			playerCount++;
		}
	}

	if (playerCount == 0)
	{
		int select = rand() % myPlayerSpawnPoints.size();
		b2Vec2 pos = myPlayerSpawnPoints.at(select);
		pos.y += 1;
		player->Respawn(pos, world);
		return;
	}

	averagePos = b2Vec2(averagePos.x / playerCount, averagePos.y / playerCount);

	float maxDist = -10000;
	b2Vec2 respawnPos;
	for (b2Vec2 pos : myPlayerSpawnPoints)
	{
		b2Vec2 spawnPos(pos.x / C_GlobalVariables::PPM, pos.y / C_GlobalVariables::PPM);
		float dist = std::sqrt(std::powf(spawnPos.x - averagePos.x, 2) + std::powf(spawnPos.y - averagePos.y, 2));
		if (dist > maxDist)
		{
			maxDist = dist;
			respawnPos = spawnPos;
			respawnPos.y += 0.5f;
		}
	}

	player->Respawn(respawnPos, world);
}

// PostUpdate(float dT)
//
// Description:	a post update to the level, removing/destroying entity objects 
//				
// @param	float	delta time
//
// 
// @return	NA
//
void c_Level_2::PostUpdate(float dT)
{
	std::vector<Entity*>::iterator iter = m_toRemove.begin();
	while (iter != m_toRemove.end())
	{
		delete* iter;
		iter = m_toRemove.erase(iter);
	}
	m_toRemove.clear();

	for (auto it : MyPlayers)
	{
		it->PostUpdate(dT);
	}
}

// ~c_Level_1()
//
// Description:	Destroy level 1 and all of its objects.
//				
// @param	
//
// 
// @return	NA
//
c_Level_2::~c_Level_2()
{

}