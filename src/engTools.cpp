#include "engTools.h"
#include <cmath>

using std::vector, std::cout, std::endl, std::sin, std::cos;

Scene* currentScene = nullptr;


Vertex3d::Vertex3d(Position3d pos)
{
	this->position = pos;
}


Position3d operator+(const Position3d& p1, const Position3d& p2) { return Position3d(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z); }
Position3d operator-(const Position3d& p1, const Position3d& p2) { return Position3d(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z); }
Position3d operator*(const Position3d& p1, const Position3d& p2) { return Position3d(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z); }
Position3d operator/(const Position3d& p1, const float div) { return Position3d(p1.x / div, p1.y / div, p1.z / div); }
bool operator==(const Position3d& p1, const Position3d& p2) { return (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z) ? true : false; }

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

void Position3d::rotdot(float matrix[])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			matrix[(i * 3) + j] *= (i == 0) ? this->x : ((i == 1) ? this->y : this->z); // Weird ternary stack, but just to turn 0-1-2 into x-y-z
			cout << matrix[i];
		}
		(i == 0) ? this->x : ((i == 1) ? this->y : this->z) = matrix[i] + matrix[i+1] + matrix[i+2]; 
		// This function stinks. (wrote when hungy)
	}
	return;
}

void Position3d::rotate(Rotation3d rot, Position3d pivot)
{
	if (*this == pivot) return; // If we are at the same position as the pivot, nothing will happen
	if (rot.x != 0)
	{
		float xmatrix[9] = {1, 0, 0,
							0, cos(x), -sin(x),
							0, sin(x), cos(x)};
		*this = *this - pivot;
		this->rotdot(xmatrix);
		*this = *this + pivot;
	}
	return;

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

Position3d Position3d::cameraspace()
{	
	Position3d cs;
	if (currentScene->currentCam != nullptr)
	{
		cs = *this - currentScene->currentCam->pos;
	}
	return cs;
}

//float Position3d::lengthSquared() const { return this->dot(*this); }
float Position3d::lengthSquared() const {
	return x * x + y * y + z * z; }

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

void Mesh::rotate(Rotation3d offset, Position3d pivot)
{
	for (Vertex3d& vert : this->vertices)
	{
		vert.position.rotate(offset, pivot);
	}
}

Rotation3d::Rotation3d() { x = 0; y = 0; z = 0; }

Rotation3d::Rotation3d(float x_, float y_, float z_)
{
	this->x = x_;
	this->y = y_;
	this->z = z_;
}

Camera* currentCam = nullptr;
