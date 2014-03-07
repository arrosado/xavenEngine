#include <Game/Core/Game.h>

#define MAXIMUM_FRAME_RATE 60
#define MINIMUM_FRAME_RATE 15
#define UPDATE_INTERVAL (1.0 / MAXIMUM_FRAME_RATE)
#define MAX_CYCLES_PER_FRAME (MAXIMUM_FRAME_RATE / MINIMUM_FRAME_RATE)

#define INPUT_KEY "InputManager"
#define SOUND_KEY "SoundManager"
#define GRAPHICS_KEY "GraphicsManager"
#define SCENES_KEY "SceneManager"
#define CONSOLE_KEY "ConsoleManager" 

InputManager* Game::GetInput() { 
	return (InputManager*)this->components[INPUT_KEY]; 
}

SoundManager* Game::GetSound() { 
	return (SoundManager*)this->components[SOUND_KEY]; 
}

GraphicsManager* Game::GetGraphics() { 
	return (GraphicsManager*)this->components[GRAPHICS_KEY]; 
}

GameSceneManager* Game::GetScenes() { 
	return (GameSceneManager*)this->components[SCENES_KEY]; 
}

Console* Game::GetConsole() { 
	return (Console*)this->components[CONSOLE_KEY];	
}

Game::Game() {
	this->components[INPUT_KEY] = InputManager::Instance();
	this->components[SOUND_KEY] = SoundManager::Instance();
    this->components[GRAPHICS_KEY] = GraphicsManager::Instance();
    this->components[SCENES_KEY] = GameSceneManager::Instance();
	this->components[CONSOLE_KEY] = Console::Instance();
}

Game::~Game() {

	std::map<std::string, IComponent*>::iterator p;

	for(p = this->components.begin(); p != this->components.end(); p++) {
		p->second->Quit();
		p->second->Cleanup();
		delete p->second;
	}

}

void Game::Initialize( int width, int height ) {

    this->components[INPUT_KEY]->Init(3, width, height, 100);
	this->components[SOUND_KEY]->Init(0);
	this->components[GRAPHICS_KEY]->Init(2, width, height);
	this->components[SCENES_KEY]->Init(0);
	this->components[CONSOLE_KEY]->Init(0);

}

void Game::Update( double delta ) {
#define FIXED_ITERATIONS 0
#if FIXED_ITERATIONS
	static double lastFrameTime = 0.0f;
	static double cyclesLeftOver = 0.0f;

	double updateIterations;
	
	updateIterations = ((delta - lastFrameTime) + cyclesLeftOver);
	
	if (updateIterations > (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL))
		updateIterations = (MAX_CYCLES_PER_FRAME * UPDATE_INTERVAL);
	
	while (updateIterations >= UPDATE_INTERVAL) {
		
		updateIterations -= UPDATE_INTERVAL;

		std::map<std::string, IComponent*>::iterator p;
		for(p = this->components.begin(); p != this->components.end(); p++) {
			p->second->HandleEvents();
			p->second->Update(UPDATE_INTERVAL);
		}
    }
	
	cyclesLeftOver = updateIterations;
	lastFrameTime = delta;
#else 

	static double lastFrameTime = 0.0f;
	
	std::map<std::string, IComponent*>::iterator p;

	for(p = this->components.begin(); p != this->components.end(); p++) {
		p->second->HandleEvents();
		p->second->Update((delta - lastFrameTime));
	}

	lastFrameTime = delta;

#endif
	
}

void Game::Render() {
	// Prepare for rendering new graphics on a clean buffer.
    this->GetGraphics()->PrepareNewFrame();
	// Actually Render new graphics to the clean buffer.
	this->GetScenes()->Draw();
	// Render the Console.
	this->GetConsole()->Draw();
	// Play queued sounds.
	this->GetSound()->PlaySounds();
	// Render the new buffer to the screen.
	this->GetGraphics()->RenderNewFrame();
}

void Game::UpdateInput(int key, bool keyState, int button, bool buttonState, int x, int y) {

	if (key >= 0) this->GetInput()->SetKeyState(key, keyState);
	
	this->GetInput()->UpdatePointer(button, buttonState, x, y);

}

void Game::ResizeView(int w, int h) {
	this->GetGraphics()->ResizeView(w, h);
	this->GetInput()->GetJoystick()->Calibrate(w/2, h/2, 0.0f);
}

void Game::Pause() {

}

void Game::Start() {

}

void Game::Exit( bool andSaveCurrentState ) {
	
}

void Game::SaveState() {

}

void Game::LoadState() {

}


