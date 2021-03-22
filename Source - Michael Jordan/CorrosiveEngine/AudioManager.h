#ifndef _AUDIO_MANAGER_H_
#define _AUDIO_MANAGER_H_

//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	AudioManager.h
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	AudioManager
// Description:
//		Handles the creation and processing of all audio.
//

//Dependancy includes
#include <fmod.hpp>

//Library includes
#include <string>
#include <vector>
#include <map>

class AudioManager
{
	//Member Functions:
public:
	static AudioManager& GetInstance();
	static void DestroyInstance();

	void Process(float dT);

	void CreateSound(std::string filename, FMOD_MODE mode);
	void PlayMusic(std::string soundName, bool fadeIn = false);

	void PlaySoundEffect(std::string soundName);
	void PlaySoundEffectOnce(std::string soundName);

	void StopMusic();
	void StopSoundEffects();
	void StopOneSoundEffect(std::string soundName);
	void DumpSounds();

	void SetMusicVolume(float _vol);
	void SetSoundEffectVolume(float _vol);
	
	float GetMusicVolume() { return m_BGVolume; };
	float GetSoundEffectVolume() { return m_SEVolume; };

protected:
	FMOD::Sound* GetSound(std::string soundName);

private:
	AudioManager();
	~AudioManager();

	//Member Data:
public:

protected:
	float m_BGVolume;
	float m_SEVolume;

	float m_currentBGVolume;
	bool m_isFadeIn;
private:
	static AudioManager* sm_pInstance;

	FMOD::System* m_AudioSystem;
	FMOD::ChannelGroup* m_musicChannels;
	FMOD::ChannelGroup* m_soundEffectChannels;
	
	std::map<std::string, FMOD::Sound*> m_soundMaps;
};

#endif // !_AUDIO_MANAGER_H_