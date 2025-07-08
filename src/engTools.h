#pragma once
//#include <SDL.h>
#include <vector>
//#include <cstdint>
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
class Mesh
{
public:
	Mesh();
	vector<Vertex3d> vertices; //vector of type Vertex
	vector<int> indices; //stores tri indices as 3-tuple
	void addVertex(Position3d pos);
	void addFace(vector<int> &ind);
	void move(Position3d offset);
};

