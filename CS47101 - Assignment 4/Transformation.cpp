#include "Transformation.h"

Transformation::Transformation()
{
	position = Vector3();
	rotation = Vector3();
	scale = Vector3(1, 1, 1);
}

Transformation::Transformation(const Vector3& p, const Vector3& r, const Vector3& s)
{
	position = p;
	rotation = r;
	scale = s;
}

Transformation::Transformation(const Transformation& rhs)
{
	position = rhs.position;
	rotation = rhs.rotation;
	scale = rhs.scale;
}

Transformation& Transformation::operator=(Transformation rhs)
{
	swap(rhs);
	return *this;
}

void Transformation::swap(Transformation& rhs)
{
	std::swap(position, rhs.position);
	std::swap(rotation, rhs.rotation);
	std::swap(scale, rhs.scale);
}

bool Transformation::operator==(const Transformation& rhs) const
{
	bool positionEqual = (position == rhs.position);
	bool rotationEqual = (rotation == rhs.rotation);
	bool scaleEqual = (scale == rhs.scale);

	return positionEqual && rotationEqual && scaleEqual;
}

bool Transformation::operator!=(const Transformation& rhs) const { return !(*this == rhs); }

void Transformation::transform()
{
	glTranslatef(position.x, position.y, position.z);

	glScalef(scale.x, scale.y, scale.z);

	glRotatef(rotation.z, 0, 0, 1);
	glRotatef(rotation.y, 0, 1, 0);
	glRotatef(rotation.x, 1, 0, 0);
}