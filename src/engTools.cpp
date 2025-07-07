#include "engTools.h"

using std::vector, std::cout, std::endl;


Vertex3d::Vertex3d(Position3d pos)
{
	this->position = pos;
	cout << "New vert created" << endl;
}

Position3d operator+(Position3d& p1, Position3d& p2) { return Position3d(p1.x + p2.x, p1.y + p2.y, p1.z + p2.z); }
Position3d operator-(Position3d& p1, Position3d& p2) { return Position3d(p1.x - p2.x, p1.y - p2.y, p1.z - p2.z); }
Position3d operator*(Position3d& p1, Position3d& p2) { return Position3d(p1.x * p2.x, p1.y * p2.y, p1.z * p2.z); }

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
ostream& operator<< (ostream& os, Position3d pos)
{
	return os << "[" << pos.x << " " << pos.y << " " << pos.z << "]";
}


Mesh::Mesh() { // Temp constructor code creates single preset tri
	//vertices.emplace_back(Position3d{ 0.0f, 1.0f, 0.5f });   // top
	//vertices.emplace_back(Position3d{ -0.5f, 1.0f, -0.5f }); // bottom left
	//vertices.emplace_back(Position3d{ 0.5f, 1.0f, -0.5f });  // bottom right
	this->addVertex(Position3d{ 0.0f, 1.0f, 0.5f });
	this->addVertex(Position3d{ -0.5f, 1.0f, -0.5f });
	this->addVertex(Position3d{ 0.5f, 1.0f, -0.5f });

	indices = { 0, 1, 2 };
}

void Mesh::addVertex(Position3d pos)
{
	vertices.emplace_back(Vertex3d{pos});
}

void Mesh::move(Position3d offset)
{
	
}
