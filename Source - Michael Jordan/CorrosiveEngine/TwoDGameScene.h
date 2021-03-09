#ifndef _2D_GAME_SCENE_H_
#define _2D_GAME_SCENE_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	GameScene.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	GameScene
// Description:
//		Contains main game loop.
//

//Parent Include
#include "SceneADT.h"

//Local Include
#include "Renderer.h"

//Library Includes
#include <vector>
#include <ctime>

//Forward Declaration
class Player;
class MeshEntity;
class Projectile;
class TextGenerator;
class ScaleBar;
class Camera;
class CubeMap;
class Player;
class Program;

class TwoDGameScene : public SceneADT
{
	//Member Functions:
public:
	TwoDGameScene();
	virtual ~TwoDGameScene();

	virtual bool Initialise();
	virtual void PreLoad();

	virtual void Draw();
	virtual void Process(float dT);
	virtual void Resize();

	void SpawnBullet(Model* _model, glm::vec3 _origin, glm::vec3 _direct, float _speed);
	
protected:
	void DrawHUD();
	void ProcessWave(float dt);
	void HandlePlayerOutOfBounds();

private:

	//Member Data:
public:

protected:
	Program* m_mainProgram;

	Camera* m_pMainCam;
	Camera* m_pFreeCam;
	Camera* m_pPlayerCam;

	time_t m_startTime;

	int m_waveCount;
	unsigned int m_waveMax;

	Player* m_pPlayer;
	std::vector<Projectile*> m_Bullets;

	CubeMap* m_pCubeMap;
	MeshEntity* m_pGroundMesh;

	TextGenerator* m_Technical_30;
	TextGenerator* m_Avara60;
	ScaleBar* m_playerHealth;

	unsigned int m_gameTimer;
	float m_spawnTimer;
	float m_secTimer;

	float m_xClamp;

	bool m_isPlayerLocked;

private:

};

#endif // !_2D_GAME_SCENE_H_