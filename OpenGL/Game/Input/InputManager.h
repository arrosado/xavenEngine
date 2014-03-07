#ifndef __INPUT_MANAGER_H__
#define __INPUT_MANAGER_H__

#include <Game/IComponent.h>
#include <Game/Input/JoyStick.h>
#include <map>

class InputManager : public IComponent { 
public:
	static InputManager* Instance() {
		return &m_InputManagerInstance;
	}

	~InputManager() { 
		delete this->m_joystick;
	}

	void Init(int argc, ...);

	void HandleEvents();
	void Update(double delta);
	void Draw();

	bool GetKeyState(int key) {
		return keyStates[key];
	}

	GameJoyStick* GetJoystick() {
		return m_joystick;
	}

	void SetKeyState(int key, bool keyState) {
		this->keyStates[key] = keyState;
	}

	void UpdatePointer(int pointerButton, int pointerState, int x, int y) {
		this->pointerButton = pointerButton;
		this->pointerState = pointerState;
		this->pointerX = x;
		this->pointerY = y;
	}

	void Cleanup();
	void Quit();

private:
	static InputManager m_InputManagerInstance;
	std::map<int, bool> keyStates;
	int pointerButton, pointerState, pointerX, pointerY;
	GameJoyStick* m_joystick;
protected: 
	InputManager() {
		this->pointerButton = 
		this->pointerState = 
		this->pointerX = 
		this->pointerY = 0;

		for(int i = 0; i < 256; i++)
			this->keyStates[i] = false;

		this->m_joystick = new GameJoyStick();
	};
};

#endif 