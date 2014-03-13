#include <Game/Graphics/Grid/Grid.h>

Grid::Grid(int total, int length, int space) {
	this->m_position = Vector3DfZero;
	this->m_color = Color4fGray;
	this->m_total = total;
	this->m_length = length;
	this->m_space = space;
}

Grid::Grid(int total, int length, int space, Color4f color) {
	this->m_position = Vector3DfZero;
	this->m_color = color;
	this->m_total = total;
	this->m_length = length;
	this->m_space = space;
}

Grid::Grid(int total, int length, int space, Color4f color, Vector3Df position) {
	this->m_position = position;
	this->m_color = color;
	this->m_total = total;
	this->m_length = length;
	this->m_space = space;
}

Grid::Grid(int total, int length, int space, Vector3Df position) {
	this->m_position = position;
	this->m_color = Color4fGray;
	this->m_total = total;
	this->m_length = length;
	this->m_space = space;
}

void Grid::SetPosition(float x, float y, float z) {
	this->m_position.x = x;
	this->m_position.y = y;
	this->m_position.z = z;
}

void Grid::SetColor(Color4f color)
{
	this->m_color = color;
}

void Grid::Draw(GridType type) {
	glColor4f(this->m_color.red, this->m_color.green, this->m_color.blue, this->m_color.alpha);

	glPushMatrix();

	glTranslatef(m_position.x, m_position.y, m_position.z);

		glBegin(GL_LINES);
			/* X Grid */
			if (type == XGrid) {
				for(int i = m_total; i >= -m_total; i--) {
					glVertex3f(i*m_length*m_space, 0.0f,  m_total*m_length*m_space);
					glVertex3f(i*m_length*m_space, 0.0f, -m_total*m_length*m_space);
				}
				for(int i = m_total; i >= -m_total; i--) {
					glVertex3f( m_total*m_length*m_space, 0.0f, i*m_length*m_space);
					glVertex3f(-m_total*m_length*m_space, 0.0f, i*m_length*m_space);
				}
			}
			/* Y Grid */
			if (type == YGrid) {
				for(int i = m_total; i >= -m_total; i--) {
					glVertex3f( m_total*m_length*m_space, i*m_length*m_space, 0.0f);
					glVertex3f(-m_total*m_length*m_space, i*m_length*m_space, 0.0f);
				}
				for(int i = m_total; i >= -m_total; i--) {
					glVertex3f(i*m_length*m_space,  m_total*m_length*m_space, 0.0f);
					glVertex3f(i*m_length*m_space, -m_total*m_length*m_space, 0.0f);
				}
			}
			/* Z Grid */
			if (type == ZGrid) {
				for(int i = m_total; i >= -m_total; i--) { 
					glVertex3f(0.0f, i*m_length*m_space,  m_total*m_length*m_space);
					glVertex3f(0.0f, i*m_length*m_space, -m_total*m_length*m_space);
				}
				for(int i = m_total; i >= -m_total; i--) {
					glVertex3f(0.0f,  m_total*m_length*m_space, i*m_length*m_space);
					glVertex3f(0.0f, -m_total*m_length*m_space, i*m_length*m_space);
				}
			}
			
		glEnd();

	glPopMatrix();

	if (m_total / 10 > 0) {
		int current_total = m_total;
		int current_space = m_space;
		Color4f current_color = m_color;
		
		m_total /= 10;
		m_color = Color4fBlack;
		m_space *= 10;

		glLineWidth(2.0);

		this->Draw(type);

		glLineWidth(0.5);

		m_total = current_total;
		m_color = current_color;
		m_space = current_space;
	}
}

void Grid::DrawBox(float boxSize) {
	glEnable(GL_DEPTH_TEST);
	this->Draw(XGrid);
	this->SetPosition(-boxSize, boxSize, 0.0);
	this->Draw(ZGrid);
	this->SetPosition(0.0f, boxSize, -boxSize);
	this->Draw(YGrid);
	this->SetPosition(boxSize, boxSize, 0.0);
	this->Draw(ZGrid);
	this->SetPosition(0.0f, boxSize * 2, 0.0);
	this->Draw(XGrid);
	glDisable(GL_DEPTH_TEST);
}

Grid::~Grid(){ 

}