#include <vector>
#include "engTools.h"
#include <iostream>

using std::vector, std::cout, std::endl;

Vertex3d::Vertex3d(int xPos, int yPos, int zPos) : x(xPos), y(yPos), z(zPos)
{
	cout << "New vert created" << endl;
}

Mesh::Mesh() { // Temp constructor code creates single preset tri
	vertices.emplace_back(Vertex3d{ 0.0f, 0.5f, 0.0f });  // top
	vertices.emplace_back(Vertex3d{ -0.5f, -0.5f, 0.0f }); // bottom left
	vertices.emplace_back(Vertex3d{ 0.5f, -0.5f, 0.0f });  // bottom right

	indices = { 0, 1, 2 };
}

Point2d::Point2d(int xin, int yin) //Take integer values as coords
{
	x = xin;
	y = yin;
}

Point2d operator+(Point2d& p1, Point2d& p2) { return Point2d(p1.x + p2.x, p1.y + p2.y); }
Point2d operator-(Point2d& p1, Point2d& p2) { return Point2d(p1.x - p2.x, p1.y - p2.y); }
Point2d operator*(Point2d& p1, Point2d& p2) { return Point2d(p1.x * p2.x, p1.y * p2.y); }

int edgeSideSS(Point2d& origin, Point2d& reference, Point2d& test)
{
	Point2d vecref(reference - origin);
	Point2d vectest(test - origin);
	// We need to rotate the test vector clockwise 90d to get correct results
	Point2d vecrotated(vectest.y,-vectest.x);
	Point2d result(vecref * vecrotated); // Multiplication is equivalent to dot product here
	return result.x + result.y;
}