#pragma once

#include "Renderer.h"
#include "LightingSystem.h"
#include "AudioManager.h"

#include <string>
#include <Queue>

enum LoadType {TEXTURE, CUBEMAP, MODEL, LIGHT, SOUND};

struct LoadPacket
{
	LoadType type;
	const char* renderID;

};

struct CubeMapPacket : public LoadPacket
{
	std::string textureLoc[6];
};

struct LightPacket : public LoadPacket
{
	glm::vec3 loc;
	glm::vec3 col;
};

struct SoundPacket : public LoadPacket
{
	FMOD_MODE mode;
};

class LoadManager
{
public:

	static LoadManager& GetInstance();
	static void DestroyInstance();

	void AddTextureLoad(const char* _textureLocation);
	void AddCubeMapLoad(const char* _title, std::string _textureNames[6]);
	void AddModelLoad(const char* _modelLocation);
	void AddPointLightLoad(glm::vec3 _loc, glm::vec3 _col);
	void AddSoundLoad(const char* _title, FMOD_MODE _mode);

	void ProcessQueue();
	void ClearQueue();

	unsigned int GetLoadSize() { return m_loadQueue.size(); };

protected:
	void LoadTexture(LoadPacket* _packet);
	void LoadCubeMap(CubeMapPacket* _packet);
	void LoadModel(LoadPacket* _packet);
	void LoadLight(LightPacket* _packet);
	void LoadSound(SoundPacket* _packet);

private:
	LoadManager() {};
	~LoadManager() { ClearQueue(); };
public:

protected:
	std::queue<LoadPacket*> m_loadQueue;
private:
	static LoadManager* sm_pInstance;
};

