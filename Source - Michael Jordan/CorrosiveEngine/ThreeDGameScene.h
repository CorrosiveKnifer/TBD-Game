#ifndef _3D_GAME_SCENE_H_
#define _3D_GAME_SCENE_H_

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
class PlayerThree;
class MeshEntity;
class Projectile;
class TextGenerator;
class ScaleBar;
class Camera;
class CubeMap;
class PlayerThree;
class Program;

class ThreeDGameScene : public SceneADT
{
	//Member Functions:
public:
	ThreeDGameScene();
	virtual ~ThreeDGameScene();

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

	PlayerThree* m_pPlayer;
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
	unsigned int m_lives;
	unsigned int m_kills;
	unsigned int m_shots;
	unsigned int m_healthScore;
	float m_scoreDelay;
	float m_gameDelay;
};

#endif // !_3D_GAME_SCENE_H_