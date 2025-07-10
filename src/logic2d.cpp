#include <algorithm>
#include "logic2d.h"
#include "engconfig.h"

using std::vector, std::cout, std::endl;

Point2d::Point2d() { this->x = 0; this->y = 0; }

Point2d::Point2d(int xin, int yin) : x(xin), y(yin) {}

Point2d::Point2d(Vertex3d from3d) // Convert a 3d space vert to screen space point (important!)
{
	Position3d ss;
	if (currentCam != nullptr)
	{
		ss = from3d.position - currentCam->pos;
	}
	float perspscale = perspectiveFac * (ss.y + 2); // Temporarily add offset to avoid clipping
	this->x = ss.x / perspscale;
	this->y = ss.z / perspscale;
	this->x += 960;
	this->y += 540;
}
Point2d operator+(const Point2d& p1, const Point2d& p2) { return Point2d(p1.x + p2.x, p1.y + p2.y); }
Point2d operator-(const Point2d& p1, const Point2d& p2) { return Point2d(p1.x - p2.x, p1.y - p2.y); }
Point2d operator*(const Point2d& p1, const Point2d& p2) { return Point2d(p1.x * p2.x, p1.y * p2.y); }

int edgeSideSS(const Point2d& origin, const Point2d& reference, const Point2d& test)
{
	int rx = reference.x - origin.x;
	int ry = reference.y - origin.y;
	int tx = test.x - origin.x;
	int ty = test.y - origin.y;

	// Rotate (tx, ty) clockwise by 90 degrees: (ty, -tx)
	// Dot product: rx*ty + ry*(-tx) = rx*ty - ry*tx
	return static_cast<int>(rx * ty - ry * tx);
}

Rect2d::Rect2d(int lX, int lY, int hX, int hY) : min(lX, lY), max(hX, hY) {}

Rect2d::Rect2d(Point2d v1, Point2d v2, Point2d v3) // Bounding box of 3 points
{
	this->min.x = std::max(std::min(v1.x, std::min(v2.x, v3.x )), 0);
	this->min.y = std::max(std::min(v1.y, std::min(v2.y, v3.y )), 0);
	this->max.x = std::min(std::max(v1.x, std::max(v2.x, v3.x )),1920);
	this->max.y = std::min(std::max(v1.y, std::max(v2.y, v3.y )),1080);
}
