#pragma once
#include <SDL.h>
#include <vector>
#include <cstdint>
#include <iostream>

using std::vector, std::ostream;

class Position3d // Stores 3D positions ONLY. Nearly always as used as part of a bigger part (e.g. vert)
{
public:
	double x, y, z;
	Position3d(double xPos, double yPos, double zPos);
	Position3d();
	friend ostream& operator<< (ostream& os, Position3d pos);
};

class Vertex3d
{
public:
	Vertex3d(Position3d pos);
	Position3d position; 
	void offsetPosition(Position3d offset);
};

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
	void addVertex(Position3d pos);
	void move(Position3d offset);
};

int edgeSideSS(Point2d& origin, Point2d& reference, Point2d& test);
