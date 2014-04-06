#ifndef __GRID_H__
#define __GRID_H__

#include <Game/Graphics/GraphicsCommon.h>

enum GridType {
	XGrid,
	YGrid,
	ZGrid
};

class Grid {
public: 
	Grid(int total, int length, int space);
	Grid(int total, int length, int space, Color4f color);
	Grid(int total, int length, int space, Color4f color, vec3 position);
	Grid(int total, int length, int space, vec3 position);
	~Grid();

	void Draw(GridType type);
	void DrawBox(float boxSize);

	void SetPosition(float x, float y, float z);
	void SetColor(Color4f color);

private:
	Color4f m_color;
	vec3 m_position;
	int m_total;
	int m_length;
	int m_space;
};

#endif 