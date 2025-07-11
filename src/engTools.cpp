#include "engTools.h"
#include <cmath>

using std::vector, std::cout, std::endl;

Scene* currentScene = nullptr;

Vertex3d::Vertex3d(Position3d pos)
{
	this->position = pos;
}

Position3d operator+(const Position3d& p1, const Position3d& p2) { return Position3d(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z); }
Position3d operator-(const Position3d& p1, const Position3d& p2) { return Position3d(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z); }
Position3d operator*(const Position3d& p1, const Position3d& p2) { return Position3d(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z); }
Position3d operator/(const Position3d& p1, const float div) { return Position3d(p1.x / div, p1.y / div, p1.z / div); }

void Vertex3d::offsetPosition(Position3d offset) 
{
	this->position = this->position + offset;
}

Position3d::Position3d() //if not passed a position, we just put it at origin
{
	this->x = 0;
	this->y = 0;
	this->z = 0;
}

Position3d::Position3d(double xPos, double yPos, double zPos)
{
	this->x = xPos;
	this->y = yPos;
	this->z = zPos;
}


Position3d Position3d::cross(const Position3d& operand) const
{
	return Position3d(
		y * operand.z - z * operand.y,
		z * operand.x - x * operand.z,
		x * operand.y - y * operand.x
	);
}
float Position3d::dot(const Position3d& operand) const
{
	return x * operand.x * y * operand.y + z * operand.z;
}

void Position3d::normalise()
{
	float magnitude = std::sqrt(x * x + y * y + z * z); // find magnitude by pythaagoras
	if (magnitude > 0.0f)
	{
		x /= magnitude;
		y /= magnitude;
		z /= magnitude;
	}
}


float Position3d::lengthSquared() const { return this->dot(*this); }

ostream& operator<< (ostream& os, Position3d pos)
{
	return os << "[" << pos.x << " " << pos.y << " " << pos.z << "]";
}


Mesh::Mesh() { 
	//this->addVertex(Position3d{ 0.0f, 1.0f, 0.5f });
	//this->addVertex(Position3d{ -0.5f, 1.0f, -0.5f });
	//this->addVertex(Position3d{ 0.5f, 1.0f, -0.5f });

	//indices = { 0, 1, 2 };
}

void Mesh::addVertex(Position3d pos)
{
	vertices.emplace_back(Vertex3d{pos});
}


void Mesh::addFace(vector<int>& ind)
{
	this->indices.insert(this->indices.end(), ind.begin(), ind.end());
}

void Mesh::move(Position3d offset)
{
	for (Vertex3d& vert : this->vertices)
	{
		vert.offsetPosition(offset);
	}
}

Rotation3d::Rotation3d() { x = 0; y = 0; z = 0; }

Vector3d::Vector3d() { }

Camera* currentCam = nullptr;
