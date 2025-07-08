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
	friend Position3d operator+(const Position3d& p1, const Position3d& p2);
	friend Position3d operator-(const Position3d& p1, const Position3d& p2);
	friend Position3d operator*(const Position3d& p1, const Position3d& p2);
};

class Rotation3d
{
public:
	float x, y, z;
	Rotation3d();
};

class Vector3d // 3D position & euclidian angle
{
public:
	Position3d position;
	Rotation3d rotation;
	Vector3d();

};

class Camera
{
public:
	Vector3d vec;
};

extern Camera* currentCam;

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

