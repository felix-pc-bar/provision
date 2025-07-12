#include <algorithm>
#include "logic2d.h"
#include "engconfig.h"

using std::vector, std::cout, std::endl;

Point2d::Point2d() { this->x = 0; this->y = 0; }

Point2d::Point2d(int xin, int yin) : x(xin), y(yin) {}

Point2d::Point2d(const Vertex3d& from3d)
{
	// Cache camera position locally to avoid repeated dereferencing
	const Position3d& camPos = currentScene->currentCam->pos;

	float dx = from3d.position.x - camPos.x;
	float dy = from3d.position.y - camPos.y;
	float dz = from3d.position.z - camPos.z;

	// Compute perspective scale factor
	float pf = perspectiveFac / screenheight;
	float perspscale = pf * dy;

	// Avoid division by zero or near-zero
	if (perspscale <= 0.0001f) {
		this->x = this->y = -99999;  // or sentinel value for culling
		return;
	}

	this->x = dx / perspscale + screenwidth * 0.5f;
	this->y = dz / perspscale + screenheight * 0.5f;
}

Point2d operator+(const Point2d& p1, const Point2d& p2) { return Point2d(p1.x + p2.x, p1.y + p2.y); }
Point2d operator-(const Point2d& p1, const Point2d& p2) { return Point2d(p1.x - p2.x, p1.y - p2.y); }
Point2d operator*(const Point2d& p1, const Point2d& p2) { return Point2d(p1.x * p2.x, p1.y * p2.y); }
bool operator==(const Point2d& p1, const Point2d& p2) { return (p1.x == p2.x && p1.y == p2.y); }

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

bool isTriangleOnScreen(const Point2d& p1, const Point2d& p2, const Point2d& p3, int screenWidth, int screenHeight)
{
	auto inBounds = [=](const Point2d& p) {
		return p.x >= 0 && p.x < screenWidth && p.y >= 0 && p.y < screenHeight;
		};

	return inBounds(p1) || inBounds(p2) || inBounds(p3);
}


Rect2d::Rect2d(int lX, int lY, int hX, int hY) : min(lX, lY), max(hX, hY) {}

Rect2d::Rect2d(Point2d v1, Point2d v2, Point2d v3) // Bounding box of 3 points
{
	this->min.x = std::max(std::min(v1.x, std::min(v2.x, v3.x )), 0);
	this->min.y = std::max(std::min(v1.y, std::min(v2.y, v3.y )), 0);
	this->max.x = std::min(std::max(v1.x, std::max(v2.x, v3.x )),screenwidth);
	this->max.y = std::min(std::max(v1.y, std::max(v2.y, v3.y )),screenheight);
}
