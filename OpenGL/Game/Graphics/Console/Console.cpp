#include <Game/Graphics/Console/Console.h>
#include <Game/Physics/PhysicsCommon.h>
#include <Game/Graphics/GraphicsCommon.h>
#include <cstdio>
#include <cstdarg>
#include <cstring>


Console Console::m_ConsoleInstance;


void Console::Init(int argc, ...) {

}

void Console::Update(double delta) {

}

void Console::Write(const char* message, ...) {
	char buffer[255];

    va_list arg;
    va_start(arg, message);
    vsprintf(buffer, message, arg);
    va_end(arg);

	this->m_messages.push_back(string(buffer));
}

void Console::Draw() { 
	for(int i = 0; i < m_messages.size(); i++) {
		const b2Vec2 point(0.0f, 58.0f - (i*2.5f));
		
		// Setup the projection and modelview matrices for 2D rendering.
		glMatrixMode(GL_PROJECTION);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D(0.0, 600, 0.0, 600);
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();
		glLoadIdentity();
		glTranslatef(0.0f, 0.0f, 1.0f);
		glColor3f(0.0f, 1.0f, 0.0f);
		
		void * font = GLUT_BITMAP_9_BY_15;
		for (int n = 0; n < m_messages.size(); n++) {

			glRasterPos2i(10, 600 - (15+(n*15)));

			for (string::iterator i = m_messages[n].begin(); i != m_messages[n].end(); ++i) {
				char c = *i;
				glutBitmapCharacter(font, c);
			}
		}
		glMatrixMode(GL_MODELVIEW);
		glPopMatrix();
		glMatrixMode(GL_PROJECTION);
		glPopMatrix();

		glMatrixMode(GL_MODELVIEW);

	}
	m_messages.clear();
}

void Console::HandleEvents() {

}

void Console::Cleanup() {
	if (m_messages.size())
		m_messages.clear();
}

void Console::Quit() {

}
