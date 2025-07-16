#include <algorithm>
#include "logic2d.h"
#include "engconfig.h"

using std::vector, std::cout, std::endl;

Point2d::Point2d() { this->x = 0; this->y = 0; }

Point2d::Point2d(int xin, int yin) : x(xin), y(yin) {}

Point2d::Point2d(const Vertex3d& from3d)
{
    const Position3d& camPos = currentScene->currentCam->pos;
    const Rotation3d& camRot = currentScene->currentCam->rot;

    // Translate world position to camera-relative coordinates
    float dx = from3d.position.x - camPos.x;
    float dy = from3d.position.z - camPos.z;
    float dz = from3d.position.y - camPos.y;

    // Apply inverse camera rotation (i.e. rotate world opposite to camera)
    float cosYaw = cos(-camRot.pitch);
    float sinYaw = sin(-camRot.pitch);
    float cosPitch = cos(-camRot.yaw);
    float sinPitch = sin(-camRot.yaw);
    float cosRoll = cos(-camRot.roll);
    float sinRoll = sin(-camRot.roll);

    // Yaw (around Y axis)
    float x1 = cosYaw * dx - sinYaw * dz;
    float z1 = sinYaw * dx + cosYaw * dz;
    float y1 = dy;

    // Pitch (around X axis)
    float y2 = cosPitch * y1 - sinPitch * z1;
    float z2 = sinPitch * y1 + cosPitch * z1;
    float x2 = x1;

    // Roll (around Z axis)
    float x3 = cosRoll * x2 - sinRoll * y2;
    float y3 = sinRoll * x2 + cosRoll * y2;
    float z3 = z2;

    // Perspective projection
    float pf = perspectiveFac / screenheight;
    float perspscale = pf * y3;

    if (perspscale <= 0.0001f) {
        this->x = this->y = -99999;  // sentinel
        return;
    }

    this->x = x3 / perspscale + screenwidth * 0.5f;
    this->y = z3 / perspscale + screenheight * 0.5f;
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
