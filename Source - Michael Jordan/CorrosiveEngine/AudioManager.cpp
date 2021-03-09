//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2020 Media Design School
//
// File Name    |	AudioManager.cpp
// Author(s)    |	Michael Jordan (Michael.jor8834@mediadesign.com)
// Class(es)	|	AudioManager
// Description:
//		Handles the creation and processing of all audio.
//

//Self Include
#include "AudioManager.h"

//Library Includes
#include <iostream>
#include <assert.h>

//Static Variables
AudioManager* AudioManager::sm_pInstance = 0;

//Constructor
AudioManager::AudioManager()
{
	m_BGVolume = 1.0f;
	m_currentBGVolume = m_BGVolume;
	m_isFadeIn = false;
	m_SEVolume = 1.0f;

	m_musicChannels = nullptr;
	m_soundEffectChannels = nullptr;

	if (FMOD::System_Create(&m_AudioSystem) != FMOD_OK)
	{
		std::cout << "ERROR: System creation failed." << std::endl;
		return;
	}

	if (m_AudioSystem->init(100, FMOD_INIT_NORMAL | FMOD_INIT_3D_RIGHTHANDED, 0) != FMOD_OK)
	{
		std::cout << "ERROR: System Initialisation failed." << std::endl;
		return;
	}
	
	m_AudioSystem->createChannelGroup(NULL, &m_soundEffectChannels);
	m_AudioSystem->createChannelGroup(NULL, &m_musicChannels);
}

//Destructor
AudioManager::~AudioManager()
{
	m_soundEffectChannels->release();
	m_soundEffectChannels = 0;

	m_musicChannels->release();
	m_musicChannels = 0;

	DumpSounds();

	m_AudioSystem->release();
	m_AudioSystem = 0;
	
}

//	GetInstance( )
//
//	Author: Michael Jordan
//	Description:
//		Get function for the only instance of this class. If the instance doesn't
//		exist then it will make one.
//		
//	Param:
//		- n/a
//
//	Return: AudioManager&	|	Single instance of the AudioManager
//
AudioManager& AudioManager::GetInstance()
{
	if (sm_pInstance == 0)
	{
		sm_pInstance = new AudioManager();
	}
	return *sm_pInstance;
}

//	DestroyInstance( )
//
//	Author: Michael Jordan
//	Description:
//		Destroys the single instance of this class
//		
//	Param:
//		- n/a
//
//	Return: n/a		|
//
void AudioManager::DestroyInstance()
{
	if (sm_pInstance != 0)
	{
		delete sm_pInstance;
		sm_pInstance = 0;
	}
}

//	Process( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Processes the audio system.
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void AudioManager::Process(float dT)
{
	assert(m_AudioSystem->update() == FMOD_OK);

	if (m_isFadeIn)
	{
		m_currentBGVolume += (m_BGVolume - m_currentBGVolume) * dT;
		m_musicChannels->setVolume(m_currentBGVolume);
		if (m_BGVolume <= m_currentBGVolume + 0.01f)
		{
			m_isFadeIn = false;
			m_musicChannels->setVolume(m_BGVolume);
		}
	}
}

//	CreateSound( filenamem, mode)
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Preloads the sound into the sound map for future use.
//		
//	Param:
//		- std::string	|	File name of the sound file.
//		- std::FMOD_MODE	|	Mode of the sound.
//
//	Return: n/a		|
//
void AudioManager::CreateSound(std::string filename, FMOD_MODE mode)
{
	std::string fileLoc("Resources/Audio/");
	fileLoc += filename;

	FMOD::Sound* newSound = GetSound(filename);
	
	if (newSound == 0)
	{
		//Sound doesn't exist
		if (m_AudioSystem->createSound(fileLoc.c_str(), mode, 0, &newSound) == FMOD_OK)
		{
			m_soundMaps.insert(std::pair<std::string, FMOD::Sound*>(filename, newSound));
		}
		else
		{
			std::cout << "Error: Sound failed creation" << std::endl;
		}
	}	
}

//	PlayMusic( soundName)
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Plays the sound on the Music channel.
//		
//	Param:
//		- std::string	|	name of the sound file.
//
//	Return: n/a		|
//
void AudioManager::PlayMusic(std::string soundName, bool fadeIn)
{
	//Get the sound which has the name provided.
	FMOD::Sound* newSound = GetSound(soundName);
	FMOD::Channel* newChannel;

	//Safety check: has it been created?
	if (newSound == nullptr)
	{
		std::cout << "Error: Sound doesn't exist... yet." << std::endl;
		return;
	}
	
	//Play sound on the new channel (creates the channel within playSound)
	if (m_AudioSystem->playSound(newSound, 0, false, &newChannel) != FMOD_OK)
	{
		std::cout << "Error: Sound failed to play" << std::endl;
		return;
	}

	//Assign newChannel to the sound effects channel group.
	newChannel->setChannelGroup(m_musicChannels);

	if (fadeIn)
	{
		m_currentBGVolume = 0.0f;
		m_isFadeIn = fadeIn;
		//Update volume:
		m_musicChannels->setVolume(m_currentBGVolume);
	}
	else
	{
		//Update volume:
		m_musicChannels->setVolume(m_BGVolume);
	}
}

//	PlaySoundEffect( soundName )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Plays the sound on the sound effect channel.
//		
//	Param:
//		- std::string	|	name of the sound file.
//
//	Return: n/a		|
//
void AudioManager::PlaySoundEffect(std::string soundName)
{
	//Get the sound which has the name provided.
	FMOD::Sound* newSound = GetSound(soundName);
	FMOD::Channel* newChannel;

	//Safety check: has it been created?
	if (newSound == nullptr)
	{
		std::cout << "Error: Sound doesn't exist... yet." << std::endl;
		return;
	}

	//Play sound on the new channel (creates the channel within playSound)
	if (m_AudioSystem->playSound(newSound, 0, false, &newChannel) != FMOD_OK)
	{
		std::cout << "Error: Sound failed to play" << std::endl;
		return;
	}

	//Assign newChannel to the sound effects channel group.
	newChannel->setChannelGroup(m_soundEffectChannels);

	//Update volume:
	newChannel->setVolume(m_SEVolume);
}

//	PlaySoundEffectOnce( soundName )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Checks if a sounds is already being played before playings the sound, 
//		on the soundeffect channel.
//		
//	Param:
//		- std::string	|	name of the sound file.
//
//	Return: n/a		|
//
void AudioManager::PlaySoundEffectOnce(std::string soundName)
{
	int ChannelCount; //Get channels playing count
	m_soundEffectChannels->getNumChannels(&ChannelCount);

	for (int i = 0; i < ChannelCount; i++)
	{
		FMOD::Channel* channel;
		FMOD::Sound* sound;

		//Get Channel & sound at index i
		m_soundEffectChannels->getChannel(0, &channel);
		channel->getCurrentSound(&sound);

		const int LENGTH = 32;
		char name[LENGTH];
		sound->getName(name, LENGTH);

		std::string thisSoundName = name;

		if (soundName == thisSoundName)
		{
			//FOUND! DO NOT PLAY AGAIN
			return;
		}
	}

	//Not currently being played
	PlaySoundEffect(soundName);
}

//	StopMusic( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Stops all sound on the music channel.
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void AudioManager::StopMusic()
{
	m_musicChannels->stop();
}

//	StopSoundEffects( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Stops all sound on the sound effect channel.
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void AudioManager::StopSoundEffects()
{
	m_soundEffectChannels->stop();
}

//	StopOneSoundEffect( soundName )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Stops one sound on the sound effect channel.
//		
//	Param:
//		- std::string	|	name of sound effect to search for.
//
//	Return: n/a		|
//
void AudioManager::StopOneSoundEffect(std::string soundName)
{
	int ChannelCount;
	m_soundEffectChannels->getNumChannels(&ChannelCount);

	for (int i = 0; i < ChannelCount; i++)
	{
		FMOD::Channel* channel;
		FMOD::Sound* sound;
		m_soundEffectChannels->getChannel(0, &channel);
		channel->getCurrentSound(&sound);
		const int LENGTH = 32;
		char name[LENGTH];
		sound->getName(name, LENGTH);
		std::string thisSoundName = name;

		if (soundName == thisSoundName)
		{
			channel->stop();
			return;
		}
	}
}

//	DumpSounds( )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Releases all sounds on the map.
//		
//	Param:
//		- n/a	|
//
//	Return: n/a		|
//
void AudioManager::DumpSounds()
{
	std::map<std::string, FMOD::Sound*>::iterator iter = m_soundMaps.begin();
	while (iter != m_soundMaps.end())
	{
		iter->second->release();
		iter++;
	}
	m_soundMaps.clear();
}

//	SetMusicVolume(_vol )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Updates the volume of the music channel.
//		
//	Param:
//		- float	|	new volume value (between 0.0f and 1.0f).
//
//	Return: n/a		|
//
void AudioManager::SetMusicVolume(float _vol)
{
	if (_vol > 1.0f)
	{
		m_BGVolume = 1.0f;
	}
	if (_vol < 0.0f)
	{
		m_BGVolume = 0.0f;
	}

	m_BGVolume = _vol;
	m_musicChannels->setVolume(m_BGVolume);
}

//	SetSoundEffectVolume(_vol )
//
//	Author: Michael Jordan
//	Access: public
//	Description:
//		Updates the volume of the sound effect channel.
//		
//	Param:
//		- float	|	new volume value (between 0.0f and 1.0f).
//
//	Return: n/a		|
//
void AudioManager::SetSoundEffectVolume(float _vol)
{
	if (_vol > 1.0f)
	{
		m_SEVolume = 1.0f;
	}
	if (_vol < 0.0f)
	{
		m_SEVolume = 0.0f;
	}

	m_SEVolume = _vol;
	m_soundEffectChannels->setVolume(m_SEVolume);
}

//	SetSoundEffectVolume(soundName )
//
//	Author: Michael Jordan
//	Access: protected
//	Description:
//		Gets the sound of the name provided, if it exists.
//		
//	Param:
//		- std::string	|	Name of the sound.
//
//	Return: n/a		|
//
FMOD::Sound* AudioManager::GetSound(std::string soundName)
{
	std::map<std::string, FMOD::Sound*>::iterator iter = m_soundMaps.find(soundName);

	if (iter != m_soundMaps.end())
	{
		//Found Sound
		return iter->second;
	}
	return 0;
}