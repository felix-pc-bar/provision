#pragma once
//#include <SDL.h>
#include <vector>
//#include <cstdint>
#include <iostream>

using std::vector, std::ostream;

class Position3d // Stores 3D positions ONLY. Nearly always as used as part of a bigger part (e.g. vert). Doubles as a vector.
{
public:
	double x, y, z;
	Position3d(double xPos, double yPos, double zPos);
	Position3d();

	Position3d cross(const Position3d& operand) const;
	float dot(const Position3d& operand) const;
	void normalise();
	float lengthSquared() const;
	Position3d cameraspace();

	friend ostream& operator<< (ostream& os, Position3d pos);
	friend Position3d operator+(const Position3d& p1, const Position3d& p2);
	friend Position3d operator-(const Position3d& p1, const Position3d& p2);
	friend Position3d operator*(const Position3d& p1, const Position3d& p2);
	friend Position3d operator/(const Position3d& p1, const float div);

	friend bool operator==(const Position3d& p1, const Position3d& p2);
};
		
class Rotation3d
{
public:
	float x, y, z;
	Rotation3d();
};

class Vector3d // Seems like a stupid class icl
{
public:
	Position3d position;
	Rotation3d rotation;
	Vector3d();

};

class Camera
{
public:
	//Vector3d vec;
	Position3d pos;
	Rotation3d rot;
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


class Scene
{
public:
	Camera* currentCam;
	vector<Mesh> meshes;
	vector<Camera> cams;
};


extern Scene* currentScene;

