#pragma once

#include <iostream>
#include <GL/glut.h>
#include <math.h>

class Vector3
{
	public:
		// Member Variables
		float x, y, z;

		// Constructors
		Vector3();
		Vector3(float, float, float);
		Vector3(const Vector3&);

		// Destructor
		~Vector3() {};

		// Basic Vector | Vector Math
		Vector3 operator+(const Vector3&) const;
		Vector3 operator-(const Vector3&) const;
		Vector3& operator-=(const Vector3&);
		Vector3& operator+=(const Vector3&);

		// Basic Vector | Scalar Math
		Vector3 operator*(float) const;
		Vector3 operator/(float) const;
		Vector3& operator*=(float);
		Vector3& operator/=(float);

		// Vector Operations
		float magnitude() const;
		float squareMagnitude() const;
		void normalize();
		Vector3 cross(const Vector3&) const;
		float dot(const Vector3&) const;

		// Assignment
		Vector3& operator=(Vector3);
		void swap(Vector3&);

		// Comparison
		bool operator==(const Vector3&) const;
		bool operator!=(const Vector3&) const;

		// IO
		friend std::ostream& operator<<(std::ostream&, const Vector3&);
		//friend std::istream& operator>>(std::istream&, Vector3&);

		// create Vertex in openGL space
		void glVertex() const;
};

Vector3 operator*(float, const Vector3&);
Vector3 operator/(float, const Vector3&);
