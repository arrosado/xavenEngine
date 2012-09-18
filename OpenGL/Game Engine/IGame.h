#ifndef __IGAME_H__
#define __IGAME_H__

#include <map>
#include <IComponent.h>

struct IGameEngine {
	virtual void SaveState() = 0;
	virtual void LoadState() = 0;
	
	std::map<std::string, IComponent*> components;
    
	virtual void Initialize( int width, int height ) = 0;
	virtual void Render() = 0;
	virtual void Update( double delta ) = 0;
	virtual void UpdateInput(int key, bool keyState, int button, bool buttonState, int x, int y) = 0;
	virtual void ResizeView(int w, int h) = 0;
	virtual void Start() = 0;
	virtual void Pause() = 0;
	virtual void Exit( bool andSaveCurrentState ) = 0;
	
	virtual ~IGameEngine() {};
};

// IGameEngine Factory Method.
struct IGameEngine* CreateGame();

struct IGameEngine* DestroyGame(IGameEngine* game);

#endif /* __IGAME_H__ */