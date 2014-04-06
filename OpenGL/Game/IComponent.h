#ifndef __ICOMPONENT_H__
#define __ICOMPONENT_H__

#include <stdarg.h>

struct IComponent { 
	virtual void Init(int argc, ...) = 0;
	virtual void Update(double delta) = 0;
	virtual void HandleEvents() = 0;
	virtual void Cleanup() = 0;
	virtual void Quit() = 0;
	virtual ~IComponent() {};
};

#endif 