#include "engTools.h"

using std::vector, std::cout, std::endl;

float perspectiveFac = 0.005;

Vertex3d::Vertex3d(double xPos, double yPos, double zPos) : x(xPos), y(yPos), z(zPos)
{
	cout << "New vert created" << endl;
}


ostream& operator<< (ostream& os, Vertex3d vert)
{
	return os << "[" << vert.x << " " << vert.y << " " << vert.z << "]";
}

Mesh::Mesh() { // Temp constructor code creates single preset tri
	vertices.emplace_back(Vertex3d{ 0.0f, 1.0f, 0.5f});  // top
	vertices.emplace_back(Vertex3d{ -0.5f, 1.0f, -0.5f }); // bottom left
	vertices.emplace_back(Vertex3d{ 0.5f, 1.0f, -0.5f});  // bottom right

	indices = { 0, 1, 2 };
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
	float perspscale = perspectiveFac * from3d.y;
	this->x = from3d.x / perspscale;
	this->y = from3d.z / perspscale;
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