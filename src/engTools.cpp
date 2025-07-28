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
Position3d operator*(const Position3d& p1, const float mpcand) { return Position3d(p1.x * mpcand, p1.y * mpcand, p1.z * mpcand); }
Position3d operator/(const Position3d& p1, const float div) { return Position3d(p1.x / div, p1.y / div, p1.z / div); }
bool operator==(const Position3d& p1, const Position3d& p2) { return (p1.x == p2.x && p1.y == p2.y && p1.z == p2.z) ? true : false; }

Position3d& Position3d::operator+=(const Position3d& other) {
	x += other.x;
	y += other.y;
	z += other.z;
	return *this;
}

Position3d& Position3d::operator-=(const Position3d& other) {
	x -= other.x;
	y -= other.y;
	z -= other.z;
	return *this;
}

void Vertex3d::offsetPosition(Position3d offset) 
{
	this->position = this->position + offset;
}

void Vertex3d::rotatePosition(const Rotation3d& rot, const Position3d& pivot)
{
	this->position.rotateAroundPoint(rot, pivot);
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

// just the rotation matrices hardcoded (it's not like we're gonna want skew later on, right?)
void Position3d::rotateAroundPoint(const Rotation3d& rotation, const Position3d& pivot)
{
	// Translate to origin relative to pivot
	float x = this->x - pivot.x;
	float y = this->y - pivot.y;
	float z = this->z - pivot.z;

	// Rotate around Z
	float cosZ = cos(rotation.roll);
	float sinZ = sin(rotation.roll);
	float x1 = cosZ * x - sinZ * y;
	float y1 = sinZ * x + cosZ * y;

	// Rotate around Y
	float cosY = cos(rotation.yaw);
	float sinY = sin(rotation.yaw);
	float x2 = cosY * x1 + sinY * z;
	float z1 = -sinY * x1 + cosY * z;

	// Rotate around X
	float cosX = cos(rotation.pitch);
	float sinX = sin(rotation.pitch);
	float y2 = cosX * y1 - sinX * z1;
	float z2 = sinX * y1 + cosX * z1;

	// Translate back from origin
	this->x = x2 + pivot.x;
	this->y = y2 + pivot.y;
	this->z = z2 + pivot.z;
}

void Position3d::rotateQuat(const Quaternion& q)
{
	Quaternion qv{ 0, x, y, z };
	Quaternion result = q * qv * q.inverse();
	this->x = result.x;
	this->y = result.y;
	this->z = result.z;
}

// QUATERNION (please work)
Quaternion::Quaternion() : w(1), x(0), y(0), z(0) {}
Quaternion::Quaternion(float angle, const Position3d& axis)
{
	float half = angle * 0.5f;
	float s = sin(half);
	w = cos(half);
	x = axis.x * s;
	y = axis.y * s;
	z = axis.z * s;
}

Quaternion::Quaternion(double w_, double x_, double y_, double z_) : w(w_), x(x_), y(y_), z(z_) {}

Quaternion Quaternion::operator*(const Quaternion& q) const {
	return {
		w * q.w - x * q.x - y * q.y - z * q.z,
		w * q.x + x * q.w + y * q.z - z * q.y,
		w * q.y - x * q.z + y * q.w + z * q.x,
		w * q.z + x * q.y - y * q.x + z * q.w
	};
}

Quaternion Quaternion::inverse() const
{
	return { w, -x, -y, -z };
}

void Quaternion::normalise()
{
	float mag = sqrt(w * w + x * x + y * y + z * z);
	if (mag == 0.0f) return; // Avoid division by zero

	w /= mag;
	x /= mag;
	y /= mag;
	z /= mag;
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

void Position3d::flip()
{
	this->x = -this->x;
	this->y = -this->y;
	this->z = -this->z;
}

Position3d Position3d::cameraspace() const
{	
	Position3d cs;
	if (currentScene->currentCam != nullptr)
	{
		cs = *this - currentScene->currentCam->pos; // Set cameraspace position by subtracting camera pos from this pos
		Quaternion camRotQ = currentScene->currentCam->quatIdentity.inverse();

		cs.rotateQuat(camRotQ);
	}
	return cs;
}

float Position3d::lengthSquared() const {
	return x * x + y * y + z * z; }

ostream& operator<< (ostream& os, Position3d pos)
{
	return os << "[" << pos.x << " " << pos.y << " " << pos.z << "]";
}

Material::Material()
{
	this->colour = { 1,1,1 };
}

Material::Material(float r, float g, float b)
{
	this->colour.red = r;
	this->colour.green = g;
	this->colour.blue = b;
}
Colour::Colour(float r, float g, float b) : red(r), green(g), blue(b) {}

Colour::Colour() : red(1), green(1), blue(1) {}

Colour& Colour::operator*=(const float val)
{
	red *= val;
	green *= val;
	blue *= val;
	return *this;
}
uint32_t Colour::raw()
{
	return	(0xFF << 24) |
			((uint32_t)(0xFF * this->red) << 16) |
			((uint32_t)(0xFF * this->green) << 8)|
			((uint32_t)(0xFF * this->blue));
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
	this->position += offset;
}

void Mesh::setPos(Position3d pos)
{
	Position3d offset = pos - this->position; // find the offset that will change the current pos to the new one
	this->move(offset);
}

Rotation3d operator+(const Rotation3d& p1, const Rotation3d& p2) { return Rotation3d(p1.pitch + p2.pitch, p1.yaw + p2.yaw, p1.roll + p2.roll); }
Rotation3d operator-(const Rotation3d& p1, const Rotation3d& p2) { return Rotation3d(p1.pitch - p2.pitch, p1.yaw - p2.yaw, p1.roll - p2.roll); }
Rotation3d operator*(const Rotation3d& p1, const Rotation3d& p2) { return Rotation3d(p1.pitch * p2.pitch, p1.yaw * p2.yaw, p1.roll * p2.roll); }

Rotation3d& Rotation3d::operator+=(const Rotation3d& other) {
	pitch += other.pitch;
	yaw += other.yaw;
	roll += other.roll;
	return *this;
}

void Mesh::rotate(const Rotation3d& rot, const Position3d& pivot)
{
	for (Vertex3d& vert : this->vertices)
	{
		vert.rotatePosition(rot, pivot);
	}
	this->rotation += rot;
}

void Mesh::setRotation(const Rotation3d& rot, const Position3d& pivot)
{
	Rotation3d offset = rot - this->rotation; // find the offset that will change the current pos to the new one
	this->rotate(offset);
}

void Mesh::rotate(const Rotation3d& rot)
{
	for (Vertex3d& vert : this->vertices)
	{
		vert.rotatePosition(rot, this->position);
	}
	this->rotation += rot;
}

void Mesh::setRotation(const Rotation3d& rot)
{
	Rotation3d offset = rot - this->rotation; // find the offset that will change the current pos to the new one
	this->rotate(offset);
}

void Mesh::rotateQuat(const Quaternion& q)
{
	Position3d center = this->position;
	for (Vertex3d& vert : this->vertices)
	{
		Position3d relPos = vert.position - center;
		relPos.rotateQuat(q);
		vert.position = relPos + center;
	}
}

void Mesh::rotateAxis(float angle, const Position3d& axis)
{
	Quaternion qDelta(angle, axis);
	this->quatIdentity =  qDelta * this->quatIdentity;
	this->quatIdentity.normalise();
	this->rotateQuat({ angle, axis });
}

void Mesh::calcBaseVecs() // (re)calculate forward/right/up vectors
{
	this->forward = { 0,0,1 }; // World forward
	this->forward.rotateQuat(this->quatIdentity);
	this->up = { 0,1,0 }; // World up
	this->up.rotateQuat(this->quatIdentity);
	this->right = { 1,0,0 }; // World right
	this->right.rotateQuat(this->quatIdentity);
}

Rotation3d::Rotation3d() { pitch = 0; yaw = 0; roll = 0; }

Rotation3d::Rotation3d(float x_, float y_, float z_)
{
	this->pitch = x_;
	this->yaw = y_;
	this->roll = z_;
}

Camera* currentCam = nullptr;

void Camera::rotateCam(float angle, const Position3d& axis)
{
	Quaternion qDelta(angle, axis);
	this->quatIdentity = this->quatIdentity * qDelta;
	this->quatIdentity.normalise();
}

void Camera::calcBaseVecs() 
{
	this->forward = { 0,0,1 }; // World forward
	this->forward.rotateQuat(this->quatIdentity);
	this->up = { 0,1,0 }; // World up
	this->up.rotateQuat(this->quatIdentity);
	this->right = { 1,0,0 }; // World right
	this->right.rotateQuat(this->quatIdentity);
}