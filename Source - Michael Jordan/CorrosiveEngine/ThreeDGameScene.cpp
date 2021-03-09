//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	GameScene.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	GameScene
// Description:
//		Contains main game loop.
//

//Self Include
#include "ThreeDGameScene.h"

//Local Includes
#include "PlayerThree.h"
#include "InputHandler.h"
#include "AudioManager.h"
#include "IniParser.h"
#include "Projectile.h"
#include "TextGenerator.h"
#include "ScaleBar.h"
#include "SceneManager.h"
#include "EntityCamera.h"
#include "FreeCamera.h"
#include "LightingSystem.h"
#include "CubeMap.h"
#include "Model.h"
#include "GameOverScene.h"
#include "OverlayScene.h"
#include "Program.h"
#include "DebugManager.h"
#include "MeshEntity.h"

//Default Constructor
ThreeDGameScene::ThreeDGameScene()
{

}

//Destructor
ThreeDGameScene::~ThreeDGameScene()
{
	//Delete player
	delete m_pMainCam;
	m_pMainCam = 0;
	
	delete m_playerHealth;
	m_playerHealth = 0;

	//delete m_pPlayer;
	m_pPlayer = 0;

	delete m_pCubeMap;
	m_pCubeMap = 0;

	//delete m_pGroundMesh;
	m_pGroundMesh = 0;

	delete m_pMainCam;
	m_pMainCam = 0;

	m_Avara60 = 0;
	m_Technical_30 = 0;
	
	std::vector<Projectile*>::iterator iter = m_Bullets.begin();
	while (iter != m_Bullets.end())
	{
		delete* iter;
		iter++;
	}
	m_Bullets.clear();

	AudioManager::GetInstance().StopMusic();
	LightingSystem::GetInstance().ClearLights();
	//Text Generator not owned by this
}

//	Initialise( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Sets up the scene before use. Note: This function will take a long time
//		unless the LoadScreenScene is used to preload all assests for this function.
//		
//	Param:
//		- n/a	|	
//
//	Return: bool	|	Status of the Initialisation
//
bool ThreeDGameScene::Initialise()
{
	//Render program
	m_mainProgram = &Renderer::GetInstance().CreateDefaultProgram();

	//Entity
	//Packet One
	m_pPlayerCam = new EntityCamera(Camera::CamType::Perspective);
	m_pPlayerCam->SetView(glm::vec3(0.0f, 1.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_pPlayerCam->TransitionView(glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f));
	
	m_pFreeCam = new FreeCamera(Camera::CamType::Perspective);
	m_pFreeCam->SetView(glm::vec3(0.0f, 1.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_pMainCam = m_pPlayerCam;

	//Packet Two
	m_pPlayer = new PlayerThree(m_pMainCam);
	InstantiateEntity(m_pPlayer);
	dynamic_cast<EntityCamera*>(m_pMainCam)->SetEntity(m_pPlayer);
	dynamic_cast<EntityCamera*>(m_pMainCam)->SetFollowing(true);

	std::string files[6];
	for (unsigned int i = 0; i < 6; i++)
		files[i] = "space_c0" + std::to_string(i) + ".png";

	//Packet Three
	m_pCubeMap = new CubeMap(*m_pMainCam, Renderer::GetInstance().CreateCubeMapTexture("SpaceCubeMap", files));

	//Packet Four
	if (LightingSystem::GetInstance().GetPointLightCount() != 5)
	{
		LightingSystem::GetInstance().AddPointLight(LightingSystem::GetInstance().CreatePointLight(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		LightingSystem::GetInstance().AddPointLight(LightingSystem::GetInstance().CreatePointLight(glm::vec3(-50.0f, 4.0f, -50.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		LightingSystem::GetInstance().AddPointLight(LightingSystem::GetInstance().CreatePointLight(glm::vec3(-50.0f, 4.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		LightingSystem::GetInstance().AddPointLight(LightingSystem::GetInstance().CreatePointLight(glm::vec3(50.0f, 4.0f, -50.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
		LightingSystem::GetInstance().AddPointLight(LightingSystem::GetInstance().CreatePointLight(glm::vec3(50.0f, 4.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f)));
	}
	
	m_gameDelay = 1.0f;
	m_lives = 3;
	m_kills = 0;
	m_isPlayerLocked = false;

	//Packet Five
	m_pGroundMesh = new MeshEntity(Renderer::GetInstance().CreateCubeMesh());
	m_pGroundMesh->SetTexture(Renderer::GetInstance().CreateTexture("GroundPlatform.png"));
	m_pGroundMesh->GetTransformRef().GetPositionRef() = glm::vec3(0.0f, -1.0f, 0.0f);
	m_pGroundMesh->GetTransformRef().GetScaleRef() = glm::vec3(50.0f, 0.5f, 50.0f);
	InstantiateEntity(m_pGroundMesh);

	//Default Variables
	m_xClamp = 300.0f;
	m_gameTimer = 60;
	m_spawnTimer = 4.0f;
	m_waveCount = -1;

	//Packet Six
	IniParser::GetInstance().LoadIniFile("Resources/Files/Wave.ini");
	m_waveMax = IniParser::GetInstance().GetValueAsInt("SetUp", "WaveCount");

	//Packet Seven
	m_playerHealth = new ScaleBar("ScaleBase.png", "ScaleTop.png");
	m_playerHealth->SetLocalPosition(glm::vec2(25.0f, 25.0f));
	m_playerHealth->SetValue(1.0f);

	//Packet Eight
	m_Technical_30 = &Renderer::GetInstance().CreateTextGenerator("Technical.ttf", 30);
	m_Avara60 = &Renderer::GetInstance().CreateTextGenerator("Avara.otf", 60);

	AudioManager::GetInstance().CreateSound("GameMusic.wav", FMOD_LOOP_NORMAL);
	AudioManager::GetInstance().PlayMusic("GameMusic.wav");

	m_startTime = time(0);

	glutSetCursor(GLUT_CURSOR_CROSSHAIR);

	//Preprocess
	Process(0.0f);

	return true;
}

//	PreLoad( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Sends packets to the load manager of all relevant assets that will be used by this
//		scene. Note: Do not use this function if the scene is small.
//		
//	Param:
//		- n/a	|	
//
//	Return: n/a		|
//
void ThreeDGameScene::PreLoad()
{
	std::string files[6];
	for (unsigned int i = 0; i < 6; i++)
		files[i] = "space_c0" + std::to_string(i) + ".png";

	//Load Textures
	LoadManager::GetInstance().AddTextureLoad("GroundPlatform.png");

	//Load Cube Map
	LoadManager::GetInstance().AddCubeMapLoad("SpaceCubeMap", files);

	//Load Models
	LoadManager::GetInstance().AddModelLoad("TankBase/TankBase.obj");
	LoadManager::GetInstance().AddModelLoad("Bullet/Bullet.obj");
	LoadManager::GetInstance().AddModelLoad("TankGun/TankGun.obj");

	//LoadLights
	LoadManager::GetInstance().AddPointLightLoad(glm::vec3(0.0f, 4.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	LoadManager::GetInstance().AddPointLightLoad(glm::vec3(-50.0f, 4.0f, -50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	LoadManager::GetInstance().AddPointLightLoad(glm::vec3(-50.0f, 4.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	LoadManager::GetInstance().AddPointLightLoad(glm::vec3(50.0f, 4.0f, -50.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	LoadManager::GetInstance().AddPointLightLoad(glm::vec3(50.0f, 4.0f, 50.0f), glm::vec3(1.0f, 1.0f, 1.0f));

	//Load Sounds
	LoadManager::GetInstance().AddSoundLoad("Fire_SE_0.wav", FMOD_DEFAULT);
	LoadManager::GetInstance().AddSoundLoad("Fire_SE_1.wav", FMOD_DEFAULT);
	LoadManager::GetInstance().AddSoundLoad("Fire_SE_2.wav", FMOD_DEFAULT);
	LoadManager::GetInstance().AddSoundLoad("Reload_SE.wav", FMOD_DEFAULT);
	LoadManager::GetInstance().AddSoundLoad("Treads_0.wav", FMOD_LOOP_NORMAL);
	LoadManager::GetInstance().AddSoundLoad("GameMusic.wav", FMOD_LOOP_NORMAL);
}

//	Draw( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Draws all entities and objects within the scene.
//		
//	Param:
//		- n/a	|	
//
//	Return: n/a		|
//
void ThreeDGameScene::Draw()
{
	//m_pCubeMap->Draw();

	SceneADT::DrawEntities(m_pMainCam);
	//m_pGroundMesh->Draw(m_pMainCam, m_mainProgram);
	
	//Draw foreground
	//m_pPlayer->Draw(m_pMainCam, m_mainProgram);

	//for (Projectile* bullet : m_Bullets)
	//{
	//	bullet->Draw(m_pMainCam, m_mainProgram);
	//}

	DrawHUD();
}

//	Process( dT )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes all entities and objects within the scene since the last call.
//		
//	Param:
//		- float		|	change in time since the last call.	
//
//	Return: n/a		|
//
void ThreeDGameScene::Process(float dT)
{
	//m_pCubeMap->Process();

	//Input delay when transitioning into this scene
	if (m_gameDelay > 0)
	{
		m_gameDelay -= dT;
	}
	
	//Check if game win
	//if (m_pEnemies.GetBoidCount() <= 0 && m_waveCount + 1 >= static_cast<int>(m_waveMax))
	//{
	//	unsigned int kills = m_kills;
	//	float accuracy = (m_shots != 0) ? m_kills / m_shots : 0.0f;
	//	double seconds = std::difftime(time(0), m_startTime);
	//	SceneManager::PopScene();
	//	SceneManager::PushScene(new GameOverScene(true, kills, seconds, accuracy));
	//	return;
	//}

	if (InputHandler::GetInstance().IsKeyPressedFirst('`') || InputHandler::GetInstance().IsKeyPressedFirst('~'))
	{
		DebugManager::GetInstance().StartDebug();
	}

	if (DebugManager::GetInstance().GetCheatState("TFC") && m_pMainCam != m_pFreeCam)
	{
		m_pMainCam = m_pFreeCam;
		dynamic_cast<FreeCamera*>(m_pFreeCam)->Activate();
		View newView = m_pPlayerCam->GetView();
		newView.up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_pMainCam->SetView(newView);
	}
	else if (!DebugManager::GetInstance().GetCheatState("TFC") && m_pMainCam != m_pPlayerCam)
	{
		m_pMainCam = m_pPlayerCam;
		dynamic_cast<FreeCamera*>(m_pFreeCam)->Deactivate();
	}

	//Process to next wave
	ProcessWave(dT);

	//Update over head timer
	if (m_gameTimer != 0)
	{
		m_secTimer += dT;
		if (m_secTimer >= 2.2f) //Note: 2.5s because original was slower.
		{
			m_secTimer -= 2.2f;
			m_gameTimer--;
		}
	}

	SceneADT::ProcessEntities(dT);

	//Process player
	//if (!m_pPlayer->IsDead())
	//{
	//	m_pPlayer->Process(dT);
	//	m_pPlayer->ProcessDestruction(dT);
	//}
	//else //Prepare to respawn
	//{
	//	if (m_lives > 0)
	//	{
	//		m_lives--;
	//		m_isPlayerLocked = false;
	//		m_pPlayer->Respawn(glm::vec3(0.0f, 0.0f, 0.0f));
	//		m_pMainCam->SetView(glm::vec3(0.0f, 40.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
	//		dynamic_cast<EntityCamera*>(m_pMainCam)->SetFollowing(true);
	//	}
	//	else
	//	{
	//		unsigned int kills = m_kills;
	//		float accuracy = (m_shots != 0) ? (float)m_kills / (float)m_shots : 0.0f;
	//		double seconds = std::difftime(time(0), m_startTime);
	//		SceneManager::PopScene();
	//		SceneManager::PushScene(new GameOverScene(false, kills, seconds, accuracy));
	//		return;
	//	}
	//}
	//Check if player is out of bounds
	HandlePlayerOutOfBounds();

	//Process camera
	m_pMainCam->Process(dT);

	//Process enemies

	//Create projectile if able (Enables holding click)
	//if (!m_isPlayerLocked && m_gameDelay <= 0 && InputHandler::GetInstance().IsMousePressedFirst(GLUT_LEFT_BUTTON))
	//{
	//	m_pPlayer->Fire(this);
	//}

	//Process all bullets
	//std::vector<Projectile*>::iterator iter = m_Bullets.begin();
	//while (iter != m_Bullets.end())
	//{
	//	(*iter)->Process(dT);
	//	
	//	if (m_pEnemies.CheckBoidCollision(**iter))
	//	{	//Enemy collision
	//		m_kills++;
	//		delete* iter;
	//		iter = m_Bullets.erase(iter);
	//	}
	//	else if (abs(glm::distance((*iter)->GetLocalPosition(), glm::vec3(0.0f, 0.0f, 0.0f))) > 65.0f)
	//	{	//Despawn bullet
	//		delete* iter;
	//		iter = m_Bullets.erase(iter);
	//	}
	//	else
	//		iter++;
	//}

	//Enemy colliding with player
	//if (m_pEnemies.CheckBoidCollision(*m_pPlayer, false))
	//{
	//	m_pPlayer->HandleDamage(0.1f);
	//}

	//Update HUD
	m_playerHealth->SetValue((float)m_pPlayer->GetHealth() / 100.0f);
	m_playerHealth->Process(dT);
}

//	Resize( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Called via the scene manager to alert that the window has resized.
//		
//	Param:
//		- n/a	|	
//
//	Return: n/a		|
//
void ThreeDGameScene::Resize()
{
	m_pMainCam->RemakeMatrix();
	//m_playerHealth->Resize();
}

//	SpawnBullet( _model, _pos, _direct, _speed)
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Creates a projectile and adds it to the games' collection of bullets.
//		
//	Param:
//		- Model*		|	Model of the bullet
//		- glm::vec3		|	Starting position of the bullet
//		- glm::vec3		|	Direction of the bullet
//		- float			|	Speed of the bullet
//
//	Return: n/a		|
//
void ThreeDGameScene::SpawnBullet(Model* _model, glm::vec3 _origin, glm::vec3 _direct, float _speed)
{
	//Projectile* bullet = new Projectile(_model, _direct, _speed);
	//bullet->SetLocalPosition(_origin);
	//bullet->SetLocalScale(glm::vec3(1.8f, 1.8f, 1.8f));
	//bullet->SetFriendly(true);
	//m_shots++;
	//m_Bullets.push_back(bullet);
}

//	DrawHUD()
//
//	Author: Michael Jordan
//	Access: protected
//	Description:
//		Draws all hud elements
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void ThreeDGameScene::DrawHUD()
{
	//HP
	//m_playerHealth->Draw();

	glm::vec2 startPos = glm::vec2(0.0f, 0.0f);
	m_Technical_30->SetColour(WhiteText);
	m_Avara60->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));

	for (unsigned int i = 0; i < 2; i++)
	{
		//Bottem Text
		m_Technical_30->RenderText("Lives: "+ std::to_string(m_lives), startPos + glm::vec2(200, 30));
		m_Technical_30->RenderText("Kills: " + std::to_string(m_kills), startPos + glm::vec2(325, 30));
		//m_Technical_30->RenderText("Enemies:"+ std::to_string(m_pEnemies.GetBoidCount()), startPos + glm::vec2(475, 30));
		m_Technical_30->RenderText("Wave:" + std::to_string(m_waveCount + 1) + "/"+ std::to_string(m_waveMax), startPos + glm::vec2(650, 30));
		m_Technical_30->SetColour(BlackText);
		startPos -= glm::vec2(3.0f, 3.0f);
	}

	//if (m_pEnemies.GetBoidCount() <= 0 && m_spawnTimer > 0)
	//{
	//	//Wave timer
	//	glm::vec2 offset = m_Avara60->GetDimentionsOfText((std::to_string((int)m_spawnTimer))) * 0.5f;
	//	m_Avara60->RenderText(std::to_string((int)m_spawnTimer), glm::vec2(Renderer::SCREEN_WIDTH * 0.50f - offset.x, Renderer::SCREEN_HEIGHT * 0.85f));
	//}
}

//	ProcessWave( dt)
//
//	Author: Michael Jordan
//	Access: protected
//	Description:
//		Processes the wave progression by checking if all enemies have been
//		removed, then spawns the next wave.
//		
//	Param:
//		- float	|	time since last call
//
//	Return: n/a		|
//
void ThreeDGameScene::ProcessWave(float dt)
{
	//if (m_pEnemies.GetBoidCount() == 0)
	//{
	//	m_spawnTimer -= dt; //Count down timer
	//	if (m_spawnTimer <= 0)
	//	{
	//		m_waveCount++;
	//
	//		IniParser::GetInstance().LoadIniFile("Resources/Files/Wave.ini");
	//		std::string wave = "Wave" + std::to_string(m_waveCount);
	//		unsigned int enemies = IniParser::GetInstance().GetValueAsInt(wave, "Normal");
	//		//Start spawning the wave
	//		for (unsigned int i = 0; i < enemies; i++)
	//		{
	//			BoidEntity* m_boidTest = new BoidEntity();
	//			m_boidTest->Initialize();
	//			m_boidTest->SetWander();
	//			glm::vec3 position;
	//			do
	//			{
	//				float randX = float(rand() % 10000) / 100.0f - 50;
	//				float randZ = float(rand() % 10000) / 100.0f - 50;
	//				position = glm::vec3(randX, 0.0f, randZ);
	//			} while (glm::distance(m_pPlayer->GetLocalPosition(), position) < 5.0f);
	//
	//			m_boidTest->SetLocalPosition(position);
	//			m_boidTest->SetPursuit(*m_pPlayer);
	//			m_pEnemies.AddBoid(m_boidTest);
	//		}
	//		
	//		m_spawnTimer = 4.0f;
	//	}
	//}
}

//	HandlePlayerOutOfBounds( )
//
//	Author: Michael Jordan
//	Access: protected
//	Description:
//		Checks if the player has gone out of bounds, if so
//		it automatically destroys the player.
//		
//	Param:
//		- float	|
//
//	Return: n/a		|
//
void ThreeDGameScene::HandlePlayerOutOfBounds()
{
	if (!m_pPlayer->IsDestroyed())
	{
		glm::vec3 pos = m_pPlayer->GetTransform().GetPosition();

		if ((pos.x <= -50 || pos.x > 50) || (pos.z <= -50 || pos.z > 50))
		{
			m_isPlayerLocked = true;
			m_pPlayer->StartFalling();
			m_pPlayer->HandleDamage(200.0f); //Lethal damage
			dynamic_cast<EntityCamera*>(m_pMainCam)->SetFollowing(false);
		}
	}
}
