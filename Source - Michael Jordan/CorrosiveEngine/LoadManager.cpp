#include "LoadManager.h"


LoadManager* LoadManager::sm_pInstance = nullptr;

LoadManager& LoadManager::GetInstance()
{
	if (sm_pInstance == nullptr)
	{
		sm_pInstance = new LoadManager();
	}

	return *sm_pInstance;
}

void LoadManager::DestroyInstance()
{
	if (sm_pInstance != nullptr)
	{
		delete sm_pInstance;
	}
	sm_pInstance = nullptr;
}

void LoadManager::AddTextureLoad(const char* _textureLocation)
{
	LoadPacket* packet = new LoadPacket();
	packet->type = TEXTURE;
	packet->renderID = _textureLocation;

	m_loadQueue.push(packet);
}

void LoadManager::AddCubeMapLoad(const char* _title, std::string _textureNames[6])
{
	CubeMapPacket* packet = new CubeMapPacket();
	packet->type = CUBEMAP;
	packet->renderID = _title;
	
	for(unsigned int i = 0; i < 6; i++)
		packet->textureLoc[i] = _textureNames[i];

	m_loadQueue.push(packet);
}

void LoadManager::AddModelLoad(const char* _modelLocation)
{
	LoadPacket* packet = new LoadPacket();
	packet->type = MODEL;
	packet->renderID = _modelLocation;

	m_loadQueue.push(packet);
}

void LoadManager::AddPointLightLoad(glm::vec3 _loc, glm::vec3 _col)
{
	LightPacket* packet = new LightPacket();
	packet->type = LIGHT;
	packet->renderID = "PointLight";
	packet->loc = _loc;
	packet->col = _col;

	m_loadQueue.push(packet);
}

void LoadManager::AddSoundLoad(const char* _title, FMOD_MODE _mode)
{
	SoundPacket* packet = new SoundPacket();
	packet->type = SOUND;
	packet->renderID = _title;
	packet->mode = _mode;

	m_loadQueue.push(packet);
}

void LoadManager::ProcessQueue()
{
	LoadPacket* topPacket = m_loadQueue.front();
	m_loadQueue.pop();

	switch (topPacket->type)
	{
	default:
		break;
	case LoadType::TEXTURE:
		LoadTexture(topPacket);
		break;
	case LoadType::CUBEMAP:
		LoadCubeMap(reinterpret_cast<CubeMapPacket*>(topPacket));
		break;
	case LoadType::MODEL:
		LoadModel(topPacket);
		break;
	case LoadType::LIGHT:
		LoadLight(reinterpret_cast<LightPacket*>(topPacket));
		break;
	case LoadType::SOUND:
		LoadSound(reinterpret_cast<SoundPacket*>(topPacket));
		break;
	}
}

void LoadManager::ClearQueue()
{
	while (!m_loadQueue.empty())
	{
		LoadPacket* topPacket = m_loadQueue.front();
		m_loadQueue.pop();
		delete topPacket;
	}
}

void LoadManager::LoadTexture(LoadPacket* _packet)
{
	Renderer::GetInstance().CreateTexture(_packet->renderID);
	delete _packet;
}

void LoadManager::LoadCubeMap(CubeMapPacket* _packet)
{
	Renderer::GetInstance().CreateCubeMapTexture(_packet->renderID, _packet->textureLoc);
	delete _packet;
}

void LoadManager::LoadModel(LoadPacket* _packet)
{
	Renderer::GetInstance().CreateModel(_packet->renderID);
	delete _packet;
}

void LoadManager::LoadLight(LightPacket* _packet)
{
	if (_packet->renderID == "PointLight")
	{
		LightingSystem::GetInstance().AddPointLight(LightingSystem::GetInstance().CreatePointLight(_packet->loc, _packet->col));
	}
	delete _packet;
}

void LoadManager::LoadSound(SoundPacket* _packet)
{
	AudioManager::GetInstance().CreateSound(_packet->renderID, _packet->mode);
	delete _packet;
}
