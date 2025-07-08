#include <algorithm>
#include "logic2d.h"

using std::vector, std::cout, std::endl;

Point2d::Point2d() { this->x = 0; this->y = 0; }

Point2d::Point2d(int xin, int yin) : x(xin), y(yin) {}

Point2d::Point2d(Vertex3d from3d) // Convert a 3d space vert to screen space point (important!)
{
	float perspscale = perspectiveFac * from3d.position.y;
	this->x = from3d.position.x / perspscale;
	this->y = from3d.position.z / perspscale;
	this->x += 960;
	this->y += 540;
}
Point2d operator+(const Point2d& p1, const Point2d& p2) { return Point2d(p1.x + p2.x, p1.y + p2.y); }
Point2d operator-(const Point2d& p1, const Point2d& p2) { return Point2d(p1.x - p2.x, p1.y - p2.y); }
Point2d operator*(const Point2d& p1, const Point2d& p2) { return Point2d(p1.x * p2.x, p1.y * p2.y); }

int edgeSideSS(const Point2d& origin, const Point2d& reference, const Point2d& test)
{
	float rx = reference.x - origin.x;
	float ry = reference.y - origin.y;
	float tx = test.x - origin.x;
	float ty = test.y - origin.y;

	// Rotate (tx, ty) clockwise by 90 degrees: (ty, -tx)
	// Dot product: rx*ty + ry*(-tx) = rx*ty - ry*tx
	return static_cast<int>(rx * ty - ry * tx);
}

Rect2d::Rect2d(int lX, int lY, int hX, int hY) : min(lX, lY), max(hX, hY) {}

Rect2d::Rect2d(Point2d v1, Point2d v2, Point2d v3)
{
	this->min.x = std::min(v1.x, std::min(v2.x, v3.x));
	this->min.y = std::min(v1.y, std::min(v2.y, v3.y));
	this->max.x = std::max(v1.x, std::max(v2.x, v3.x));
	this->max.y = std::max(v1.y, std::max(v2.y, v3.y));
}
