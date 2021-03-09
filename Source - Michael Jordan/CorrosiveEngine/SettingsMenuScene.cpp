//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	SettingsMenuScene.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	SettingsMenuScene
// Description:
//		Settings scene to alter the global setting like volume.
//
//Self Include
#include "SettingsMenuScene.h"

//Local Includes
#include "Mesh.h"
#include "Renderer.h"
#include "Button.h"
#include "InputHandler.h"
#include "AudioManager.h"
#include "SceneManager.h"
//#include "GameScene.h"
#include "ScaleBar.h"
#include "IniParser.h"
#include "TextGenerator.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Canvas.h"
#include "Program.h"

//Default Constructor
SettingsMenuScene::SettingsMenuScene(bool fromNetwork)
{
	m_fromNetwork = fromNetwork;
}

//Destructor
SettingsMenuScene::~SettingsMenuScene()
{
	//Update Settings upon closing of this scene.
	if (IniParser::GetInstance().LoadIniFile("Resources/Files/Settings.ini"))
	{
		int bg = static_cast<int>(AudioManager::GetInstance().GetMusicVolume() * 100.0f);
		int se = static_cast<int>(AudioManager::GetInstance().GetSoundEffectVolume() * 100.0f);

		IniParser::GetInstance().SetValueAsString("Setting", "BG", std::to_string(bg));
		IniParser::GetInstance().SetValueAsString("Setting", "SE", std::to_string(se));

		IniParser::GetInstance().SaveIniFile();
	}

	m_textGen35 = 0;
	m_textGen15 = 0;

	delete m_pCubeMap;
	m_pCubeMap = 0;

	delete m_pMainCam;
	m_pMainCam = 0;

	delete m_pCanvas;
	m_pCanvas = 0;

	//TextGenerators are deleted within the Renderer class.
}

bool SettingsMenuScene::Initialise()
{
	//Render program
	m_program = &Renderer::GetInstance().CreateDefaultProgram();

	m_timeElapsed = 0.0f;

	//Background
	//std::string files[6];
	//for (unsigned int i = 0; i < 6; i++)
	//	files[i] = "space_c0" + std::to_string(i) + ".png";
	//

	m_pMainCam = new Camera(Camera::CamType::Perspective);
	//Note: SpaceCubeMap preloaded in Main menu
	m_pCubeMap = new CubeMap(*m_pMainCam, Renderer::GetInstance().CreateCubeMapTexture("SpaceCubeMap", nullptr));

	m_pCanvas = new Canvas();

	Button* _backButton = new Button("Back", "ButtonBase.png");
	_backButton->SetMouseClickCallBack(std::bind(&SettingsMenuScene::Back, this));
	m_pCanvas->AddWidget("Back_Button", _backButton, ALIGN_TOP_CENTRE, ALIGN_BOTTEM_CENTRE, glm::vec2(0.0f, 0.0f));

	ScaleBar* _backgroundBar = new ScaleBar("ScaleBase.png", "ScaleTop.png");
	//_backgroundBar->SetMouseClickCallBack();
	m_pCanvas->AddWidget("BG", _backgroundBar, ALIGN_CENTRE_LEFT, ALIGN_CENTRE, glm::vec2(0.0f, 175.0f));

	ScaleBar* _soundEffectBar = new ScaleBar("ScaleBase.png", "ScaleTop.png");

	m_pCanvas->AddWidget("SE", _soundEffectBar, ALIGN_CENTRE_LEFT, ALIGN_CENTRE, glm::vec2(0.0f, 75.0f));

	//Sound to be played when the SE bar moves
	AudioManager::GetInstance().CreateSound("Fire_SE_1.wav", FMOD_DEFAULT);

	m_hasReturned = false;

	//Text Generators
	m_textGen15 = &Renderer::GetInstance().CreateTextGenerator("Forward.ttf", 15);
	m_textGen35 = &Renderer::GetInstance().CreateTextGenerator("Forward.ttf", 35);

	return true;
}

void SettingsMenuScene::PreLoad()
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
void SettingsMenuScene::Draw()
{
	m_pCubeMap->Draw(m_pMainCam, nullptr);

	//X location to base all other locations from
	float titleX = (Renderer::SCREEN_WIDTH - m_textGen35->GetDimentionsOfText("SETTINGS").x) * 0.5f;

	//Set to white
	m_textGen35->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	
	m_textGen35->RenderText("SETTINGS", glm::vec2(titleX, 650.0f));

	m_textGen15->RenderText("Background Volume: ", glm::vec2(160, 600.0f));
	m_textGen15->RenderText("SoundEffect Volume: ", glm::vec2(160, 500.0f));

	//Render Widgets
	//m_pBackButton->Draw();
	m_pCanvas->Draw();
	//m_SEBar->Draw();

	//Render Controls:
	m_textGen15->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));

	titleX = (Renderer::SCREEN_WIDTH - m_textGen35->GetDimentionsOfText("~ CONTROLS ~").x) * 0.5f;
	m_textGen35->RenderText("~ CONTROLS ~", glm::vec2(titleX, 370.0f));

	m_textGen15->RenderText(" - W/S to Move forward/backwards.", glm::vec2(160, 335.0f));
	m_textGen15->RenderText(" - A/D to turn (only while moving).", glm::vec2(160, 310.0f));
	m_textGen15->RenderText(" - Mouse to aim.", glm::vec2(160, 285.0f));
	m_textGen15->RenderText(" - Left Click to shoot.", glm::vec2(160, 260.0f));
	m_textGen15->RenderText(" - Esc to pause.", glm::vec2(160, 235.0f));

	m_textGen15->RenderText("Beware: Do not fall off the platform.", glm::vec2(160, 180.0f));
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
void SettingsMenuScene::Process(float dT)
{	
	if (m_hasReturned)
	{
		SceneManager::PopScene(true); //Exit this scene
		return;
	}

	m_timeElapsed += dT / 2.0f;

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
void SettingsMenuScene::Resize()
{
	m_pMainCam->RemakeMatrix();
	//m_pBackButton->Resize();
	//m_BGBar->Resize();
	//m_SEBar->Resize();
}

void SettingsMenuScene::Back()
{
	m_hasReturned = true;
	return; 
}
