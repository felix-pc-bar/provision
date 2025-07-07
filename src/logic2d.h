#pragma once
#include "engTools.h"
#include "engconfig.h"

class Point2d
{
public:
	Point2d(Vertex3d from3d);
	Point2d(int xin, int yin);
	friend Point2d operator+(Point2d& p1, Point2d& p2);
	int x, y;
};

Point2d operator+(Point2d& p1, Point2d& p2);
Point2d operator-(Point2d& p1, Point2d& p2);
Point2d operator*(Point2d& p1, Point2d& p2);

int edgeSideSS(Point2d& origin, Point2d& reference, Point2d& test);
