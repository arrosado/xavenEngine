#ifndef __GRID_H__
#define __GRID_H__

#include <Graphics Engine\GraphicsCommon.h>

enum GridType {
	XGrid,
	YGrid,
	ZGrid
};

class Grid {
public: 
	Grid(int total, int length, int space);
	Grid(int total, int length, int space, Color4f color);
	Grid(int total, int length, int space, Color4f color, Vector3Df position);
	Grid(int total, int length, int space, Vector3Df position);
	~Grid();

	void Draw(GridType type);
	void Grid::DrawGridBox(float boxSize);

	void SetPosition(float x, float y, float z);

private:
	Color4f m_color;
	Vector3Df m_position;
	int m_total;
	int m_length;
	int m_space;
};

#endif 