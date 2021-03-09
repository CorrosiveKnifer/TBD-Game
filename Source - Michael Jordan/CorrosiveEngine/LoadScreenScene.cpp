#include "LoadScreenScene.h"
#include "Label.h"
#include "LoadManager.h"
#include "SceneManager.h"

LoadScreenScene::LoadScreenScene(SceneADT* _newScene)
{
	m_pCanvas = nullptr;
	m_scene = _newScene;
	m_packetTotal = 0;
}

LoadScreenScene::~LoadScreenScene()
{

}

bool LoadScreenScene::Initialise()
{
	m_pCanvas = new Canvas();

	m_scene->PreLoad();
	m_packetTotal = LoadManager::GetInstance().GetLoadSize();

	Label* _titleLabel = new Label("LOADING", "Avara.otf", 60);
	_titleLabel->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	m_pCanvas->AddWidget("Title", _titleLabel, ALIGN_TOP_CENTRE, ALIGN_TOP_CENTRE, glm::vec2(0.0f, -150.0f));

	Label* _descriptionlabel = new Label("Items: 0/"+std::to_string(m_packetTotal), "Forward.ttf", 15);
	_descriptionlabel->SetColour(glm::vec3(1.0f, 1.0f, 1.0f));
	m_pCanvas->AddWidget("Description", _descriptionlabel, ALIGN_BOTTEM_CENTRE, ALIGN_BOTTEM_CENTRE, glm::vec2(0.0f, 50.0f));

	return true;
}

void LoadScreenScene::PreLoad()
{
	//Do Nothing
}

void LoadScreenScene::Draw()
{
	m_pCanvas->Draw();
}

void LoadScreenScene::Process(float dT)
{
	m_pCanvas->Process(dT);

	unsigned int currentLoad = LoadManager::GetInstance().GetLoadSize();
	if (currentLoad == 0)
	{
		SceneADT* _scene = m_scene;
		SceneManager::PopScene();
		SceneManager::PushScene(_scene, true);
		return;
	}

	LoadManager::GetInstance().ProcessQueue();

	dynamic_cast<Label*>(m_pCanvas->GetWidget("Description"))->SetText(
		"Items: "+ std::to_string(m_packetTotal - currentLoad)+"/" + std::to_string(m_packetTotal)
	);
}

void LoadScreenScene::Resize()
{
	m_pCanvas->Resize();
}