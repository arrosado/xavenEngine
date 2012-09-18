#include <Sound Engine\SoundCommon.h>
#include <Sound Engine\SoundManager.h>

SoundManager SoundManager::m_SoundManagerInstance;

void SoundManager::Init(int argc, ...) { }
void SoundManager::AddSoundToPlayingQueue(SoundDetails *soundDetails) {}
void SoundManager::PlaySounds() {}
void SoundManager::Update(double delta) {}
void SoundManager::HandleEvents() {}
void SoundManager::Cleanup() {}