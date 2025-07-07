#include "engTools.h"

using std::vector, std::cout, std::endl;

float perspectiveFac = 0.005;

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

Point2d::Point2d(int xin, int yin) //Take integer values as coords
{
	this->x = xin;
	this->y = yin;
}

/*
Consider branching positional spaces further than just world space -> screen space. 
Ignoring camera space later, we should include "screen space" (as ranging -1 to 1 or something - careful of stretching?)
then we want 'display space' which works with the pixel values, _flipped vertically._
*/

Point2d::Point2d(Vertex3d from3d) // Convert a 3d space vert to screen space point (important!)
{
	float perspscale = perspectiveFac * from3d.position.y;
	this->x = from3d.position.x / perspscale;
	this->y = from3d.position.z / perspscale;
	this->x += 960;
	this->y += 540;
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
	//cout << result.x << result.y << endl;
	//cout << origin.x + origin.y << endl;
	return result.x + result.y;
}

