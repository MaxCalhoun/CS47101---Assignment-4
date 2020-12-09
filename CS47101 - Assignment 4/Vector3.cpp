#include "Vector3.h"

Vector3::Vector3()
{
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
}

Vector3::Vector3(float x_, float y_, float z_)
{
	x = x_;
	y = y_;
	z = z_;
}

Vector3::Vector3(const Vector3& rhs)
{
	x = rhs.x;
	y = rhs.y;
	z = rhs.z;
}

Vector3 Vector3::operator+(const Vector3& rhs) const
{
	Vector3 ret;

	ret.x = x + rhs.x;
	ret.y = y + rhs.y;
	ret.z = z + rhs.z;

	return ret;
}

Vector3& Vector3::operator+=(const Vector3& rhs)
{
	x = x + rhs.x;
	y = y + rhs.y;
	z = z + rhs.z;

	return *this;
}

Vector3 Vector3::operator-(const Vector3& rhs) const
{
	Vector3 ret;

	ret.x = x - rhs.x;
	ret.y = y - rhs.y;
	ret.z = z - rhs.z;

	return ret;
}

Vector3& Vector3::operator-=(const Vector3& rhs)
{
	x = x - rhs.x;
	y = y - rhs.y;
	z = z - rhs.z;

	return *this;
}

Vector3 Vector3::operator*(float rhs) const
{
	Vector3 ret;

	ret.x = x * rhs;
	ret.y = y * rhs;
	ret.z = z * rhs;

	return ret;
}

Vector3& Vector3::operator*=(float rhs)
{
	x = x * rhs;
	y = y * rhs;
	z = z * rhs;

	return *this;
}

Vector3 Vector3::operator/(float rhs) const
{
	Vector3 ret;

	ret.x = x / rhs;
	ret.y = y / rhs;
	ret.z = z / rhs;

	return ret;
}

Vector3& Vector3::operator/=(float rhs)
{
	x = x / rhs;
	y = y / rhs;
	z = z / rhs;

	return *this;
}

float Vector3::magnitude() const
{
	float sMag = squareMagnitude();

	if (sMag <= 0)
		return 0;

	return sqrt(sMag);
}

float Vector3::squareMagnitude() const
{
	return x * x + y * y + z * z;
}

void Vector3::normalize()
{
	float mag = magnitude();

	x /= mag;
	y /= mag;
	z /= mag;
}

Vector3 Vector3::cross(const Vector3& rhs) const
{
	Vector3 ret;

	ret.x = (y * rhs.z) - (z * rhs.y);
	ret.y = (rhs.x * z) - (x * rhs.z);
	ret.z = (x * rhs.y) - (y * rhs.x);

	return ret;
}

float Vector3::dot(const Vector3& rhs) const
{
	return (x * rhs.x) + (y * rhs.y) + (z * rhs.z);
}

Vector3& Vector3::operator=(Vector3 rhs)
{
	swap(rhs);
	return *this;
}

void Vector3::swap(Vector3& rhs)
{
	std::swap(x, rhs.x);
	std::swap(y, rhs.y);
	std::swap(z, rhs.z);
}

bool Vector3::operator==(const Vector3& rhs) const
{
	bool xEqual = (x == rhs.x);
	bool yEqual = (y == rhs.y);
	bool zEqual = (z == rhs.z);

	return xEqual && yEqual && zEqual;
}

bool Vector3::operator!=(const Vector3& rhs) const { return !(*this == rhs); }

std::ostream& operator<<(std::ostream& out, const Vector3& rhs)
{
	out << "<" << rhs.x << ", " << rhs.y << ", " << rhs.z << ">" << std::endl;

	return out;
}

void Vector3::glVertex() const { glVertex3f(x, y, z); }

Vector3 operator*(float lhs, const Vector3& rhs) { return rhs * lhs; }
Vector3 operator/(float lhs, const Vector3& rhs) { return rhs / lhs; }