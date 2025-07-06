#pragma once
#include <SDL.h>
#include <vector>
#include <cstdint>
#include <iostream>

using std::vector, std::ostream;

class Vertex3d
{
public:
	Vertex3d(double xPos, double yPos, double zPos);
	double x, y, z;
	friend ostream& operator<< (ostream& os, Vertex3d vert);
};

ostream& operator<< (ostream& os, Vertex3d vert);

class Point2d
{
public:
	Point2d(Vertex3d from3d);
	Point2d(int xin, int yin);
	friend Point2d operator+(Point2d& p1, Point2d& p2);
	int x, y;
};

Point2d operator+(Point2d& p1, Point2d& p2);
Point2d operator-(Point2d& p1, Point2d& p2);
Point2d operator*(Point2d& p1, Point2d& p2);

class Mesh
{
public:
	Mesh();
	vector<Vertex3d> vertices; //vector of type Vertex
	vector<int> indices; //stores tri indices as 3-tuple
};

int edgeSideSS(Point2d& origin, Point2d& reference, Point2d& test);
