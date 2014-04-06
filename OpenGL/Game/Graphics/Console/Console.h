#ifndef __CONSOLE_H__
#define __CONSOLE_H__

#include <Game/IComponent.h>
#include <vector>
#include <string>

using namespace std;

class Console : public IComponent { 
public: 
	static Console* Instance() { 
		return &m_ConsoleInstance;
	}

	~Console() {
		
	};

	void Init(int argc, ...);
	void Update(double delta);
	void HandleEvents();
	void Write(const char* message, ...);
	void Draw();
	void Cleanup();
	void Quit();
	
private:
	static Console m_ConsoleInstance;
	vector<string> m_messages;
protected:
	Console() {};
};

#endif