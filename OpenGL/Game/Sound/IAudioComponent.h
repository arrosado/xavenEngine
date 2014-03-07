#ifndef __IAUDIO_COMPONENT_H__
#define __IAUDIO_COMPONENT_H__

#include <Game/Sound/SoundCommon.h>
#include <string>
#include <map>


struct IAudioComponent { 
protected:
	std::map<std::string, SoundDetails*> m_sounds;
public:
	virtual void Update(float delta) = 0;
	virtual void Play() = 0;
};

#endif 