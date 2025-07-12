#pragma once
//#include <SDL.h>
#include <vector>
//#include <cstdint>
#include <iostream>

using std::vector, std::ostream;

class Position3d // Stores 3D positions ONLY. Nearly always as used as part of a bigger part (e.g. vert). Doubles as a vector.
{
private:
	void rotdot(float* matrix);
public:
	double x, y, z;
	Position3d(double xPos, double yPos, double zPos);
	Position3d();
	Position3d cameraspace();
	void rotate(Rotation3d rot, Position3d pivot); // Rotate position around specified pivot

	// Vector stuff
	Position3d cross(const Position3d& operand) const;
	float dot(const Position3d& operand) const;
	void normalise();
	float lengthSquared() const;


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
	Rotation3d(float x_, float y_, float z_);
};

class Camera
{
public:
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
	void rotate(Rotation3d offset, Position3d pivot);
};


class Scene
{
public:
	Camera* currentCam;
	vector<Mesh> meshes;
	vector<Camera> cams;
};


extern Scene* currentScene;

