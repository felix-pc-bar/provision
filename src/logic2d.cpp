#include "logic2d.h"

using std::vector, std::cout, std::endl;

Point2d::Point2d(int xin, int yin) //Take integer values as coords
{
	this->x = xin;
	this->y = yin;
}

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

