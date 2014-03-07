#ifndef __SOUND_MANAGER_H__
#define __SOUND_MANAGER_H__

#include <Game/IComponent.h>
#include <Game/Sound/SoundCommon.h>

class SoundManager : public IComponent { 
public: 
	static SoundManager* Instance() { 
		return &m_SoundManagerInstance;
	}

	void Init(int argc, ...);

	// Sound Management Methods.
	void AddSoundToPlayingQueue(SoundDetails *soundDetails);
	void PlaySounds();

	void HandleEvents();
	void Update(double delta);

	void Cleanup();
	void Quit() {}

private: 
	static SoundManager m_SoundManagerInstance;

protected: 
	SoundManager() {};
};

#endif 