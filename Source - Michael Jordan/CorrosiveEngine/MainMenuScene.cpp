//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	MainMenuScene.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	MainMenuScene
// Description:
//		Settings scene to alter the global setting like volume.
//

//Self Include
#include "MainMenuScene.h"

//Local Includes
#include "SettingsMenuScene.h"
#include "MeshEntity.h"
#include "Renderer.h"
#include "Button.h"
#include "InputHandler.h"
#include "AudioManager.h"
#include "SceneManager.h"
#include "IniParser.h"
#include "ThreeDGameScene.h"
#include "TwoDGameScene.h"
#include "Camera.h"
#include "CubeMap.h"
#include "TextGenerator.h"
#include "Canvas.h"
#include "Label.h"
#include "TextBox.h"
#include "LogManager.h"
#include "Program.h"

//Default Constructor
MainMenuScene::MainMenuScene()
{
	
}

//Destructor
MainMenuScene::~MainMenuScene()
{
	delete m_backOrb;
	m_backOrb = 0;

	delete m_pMainCam;
	m_pMainCam = 0;

	delete m_pCubeMap;
	m_pCubeMap = 0;

	m_Avara60 = 0;
	m_Avara15 = 0;
}

bool MainMenuScene::Initialise()
{
	m_program = &Renderer::GetInstance().CreateProgram("ReflectShaderVert.vs", "ReflectShaderFrag.fs");
	m_Avara60 = &Renderer::GetInstance().CreateTextGenerator("Avara.otf", 60);
	m_Avara15 = &Renderer::GetInstance().CreateTextGenerator("Avara.otf", 15);
	
	m_pMainCam = new Camera(Camera::CamType::Perspective);

	std::string files[6];
	for (unsigned int i = 0; i < 6; i++)
		files[i] = "space_c0" + std::to_string(i) + ".png";

	m_pCubeMap = new CubeMap(*m_pMainCam, Renderer::GetInstance().CreateCubeMapTexture("SpaceCubeMap", files));
	m_pCubeMap->GetTransformRef().GetScaleRef() = glm::vec3(2000.0f, 2000.0f, 2000.0f);

	if (!IniParser::GetInstance().LoadIniFile("Resources/Files/Settings.ini"))
	{
		IniParser::GetInstance().CreateIniFile("Resources/Files/Settings.ini");

		IniParser::GetInstance().AddNewValue("Setting", "BG", "100");
		IniParser::GetInstance().AddNewValue("Setting", "SE", "100");

		IniParser::GetInstance().SaveIniFile();
	}

	float BGVol = IniParser::GetInstance().GetValueAsInt("Setting", "BG") / 100.0f;
	AudioManager::GetInstance().SetMusicVolume(BGVol);

	float SEVol = IniParser::GetInstance().GetValueAsInt("Setting", "SE") / 100.0f;
	AudioManager::GetInstance().SetSoundEffectVolume(SEVol);

	m_backOrb = new MeshEntity(Renderer::GetInstance().CreateSphereMesh(60, 60));

	m_isUnloaded = false;

	m_pCanvas = new Canvas();
	Label* _label = new Label("VOID TANKS", "Avara.otf", 60);
	_label->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	m_pCanvas->AddWidget("Title", _label, ALIGN_TOP_CENTRE, ALIGN_TOP_CENTRE, glm::vec2(0.0f, -150.0f));

	Button* _play3D = new Button("Open 3D Test", "ButtonBase.png");
	_play3D->SetMouseClickCallBack(std::bind(&MainMenuScene::Open3DTest, this));
	m_pCanvas->AddWidget("3D_TEST", _play3D, ALIGN_CENTRE, ALIGN_CENTRE, glm::vec2(0.0f, 90.0f));

	Button* _play2D = new Button("Open 2D Test", "ButtonBase.png");
	_play2D->SetMouseClickCallBack(std::bind(&MainMenuScene::Open2DTest, this));
	m_pCanvas->AddWidget("2D_TEST", _play2D, ALIGN_CENTRE, ALIGN_CENTRE, glm::vec2(0.0f, 0.0f));

	Button* _settings = new Button("Settings & Controls", "ButtonBase.png");
	_settings->SetMouseClickCallBack(std::bind(&MainMenuScene::Settings, this));
	m_pCanvas->AddWidget("Settings & Controls", _settings, ALIGN_CENTRE, ALIGN_CENTRE, glm::vec2(0.0f, -230.0f));

	Button* _quit = new Button("Quit", "ButtonBase.png");
	_quit->SetMouseClickCallBack(std::bind(&MainMenuScene::Quit, this));
	m_pCanvas->AddWidget("Quit", _quit, ALIGN_CENTRE, ALIGN_CENTRE, glm::vec2(0.0f, -310.0f));

	m_pCanvas->AddWidget("InputA", new TextBox(), ALIGN_CENTRE, ALIGN_CENTRE, glm::vec2(0.0f, 0.0f));

	AudioManager::GetInstance().CreateSound("MainMenu.wav", FMOD_LOOP_NORMAL);
	AudioManager::GetInstance().PlayMusic("MainMenu.wav", true);

	m_showButtons = true;

	m_timeElapsed = 0.0f;

	return true;
}

void MainMenuScene::PreLoad()
{

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
void MainMenuScene::Draw()
{
	m_pCubeMap->Draw(m_pMainCam, nullptr);

	m_program->Use();

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_pCubeMap->GetTexture().GetID());

	m_program->SetInteger("skybox", 0);

	m_backOrb->Draw(m_pMainCam, m_program);
	//m_Avara60->RenderText("VOID TANKS", glm::vec2(175, 600));
	m_Avara15->RenderText("[~] to Show/Hide Buttons.", glm::vec2(0, 0));

	if (m_showButtons)
	{
		m_pCanvas->Draw();
	}

	m_program->Clear();
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
void MainMenuScene::Process(float dT)
{
	if (m_isUnloaded)
	{	//Reload the music if returning to the menu.
		AudioManager::GetInstance().PlayMusic("MainMenu.wav", true);
		glutSetCursor(GLUT_CURSOR_INHERIT);
		m_isUnloaded = false;
	}

	m_timeElapsed += dT / 6.0f;

	//Rotate camera
	glm::vec3 pos = m_pMainCam->GetWorldPosition();
	pos.x = sin(m_timeElapsed) * 3.0f;
	pos.y = 1.5f;
	pos.z = cos(m_timeElapsed) * 3.0f;
	m_pMainCam->Process(dT);
	m_pMainCam->SetPosition(pos);

	m_pCubeMap->Process(dT);

	m_pCanvas->Process(dT);
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
void MainMenuScene::Resize()
{
	m_pMainCam->RemakeMatrix();
	m_pCanvas->Resize();
}

void MainMenuScene::Open3DTest()
{
	m_isUnloaded = true;
	AudioManager::GetInstance().StopMusic();
	SceneManager::LoadScene(new ThreeDGameScene());
}

void MainMenuScene::Open2DTest()
{
	m_isUnloaded = true;
	AudioManager::GetInstance().StopMusic();
	SceneManager::LoadScene(new TwoDGameScene());
}

void MainMenuScene::Settings()
{
	SceneManager::PushScene(new SettingsMenuScene(), true);
}

void MainMenuScene::Quit()
{
	m_isUnloaded = true;
	AudioManager::GetInstance().StopMusic();
	glutLeaveMainLoop();
}
