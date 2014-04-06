#include <Game/Input/InputManager.h>

InputManager InputManager::m_InputManagerInstance;

void InputManager::Init(int argc, ...) {

	va_list l;
	va_start(l, argc);

	int width = va_arg(l, int);// width;
	int height = va_arg(l, int);// height;
	//int joystickSensitivity = va_arg(l, int); //

	va_end(l);

	this->m_joystick->Initialize(vec3(width/2, height/2, 0.0f), 100);
}

void InputManager::HandleEvents() { 

} 

void InputManager::Update(double delta) { 
	this->m_joystick->Update(vec3(this->pointerX, this->pointerY, 0.0f));
}

void InputManager::Draw() { }

void InputManager::Cleanup() { 
	this->keyStates.clear();
	this->keyStates.empty();
}

void InputManager::Quit() {
	delete this->m_joystick;
}