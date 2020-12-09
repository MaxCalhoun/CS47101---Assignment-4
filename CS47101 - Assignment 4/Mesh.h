#pragma once

#include <functional>
#include <GL/glut.h>
#include "Vector3.h"

typedef std::function<void(void)> Mesh;

// Sphere
static const Mesh standardSphere = []() { glutSolidSphere(0.5, 20, 20); };
static const Mesh standardWireSphere = []() { glutWireSphere(0.5, 20, 20); };

// Cube
static const Mesh standardCube = []() { glutSolidCube(1); };
static const Mesh standardWireCube = []() { glutWireCube(1); };
static const Mesh standardTextureCube = []()
{
	// Top Face
	glBegin(GL_QUADS);
	{
		Vector3(-0.5, 0.5, -0.5).glVertex(); glTexCoord2f(0, 0);
		Vector3(0.5, 0.5, -0.5).glVertex(); glTexCoord2f(1, 0);
		Vector3(0.5, 0.5, 0.5).glVertex(); glTexCoord2f(1, 1);
		Vector3(-0.5, 0.5, 0.5).glVertex(); glTexCoord2f(0, 1);
	}
	glEnd();

	// Bottom Face
	glBegin(GL_QUADS);
	{
		Vector3(-0.5, -0.5, -0.5).glVertex(); glTexCoord2f(0, 0);
		Vector3(0.5, -0.5, -0.5).glVertex(); glTexCoord2f(1, 0);
		Vector3(0.5, -0.5, 0.5).glVertex(); glTexCoord2f(1, 1);
		Vector3(-0.5, -0.5, 0.5).glVertex(); glTexCoord2f(0, 1);
	}
	glEnd();

	// Left Face
	glBegin(GL_QUADS);
	{
		Vector3(-0.5, -0.5, 0.5).glVertex(); glTexCoord2f(0, 0);
		Vector3(-0.5, -0.5, -0.5).glVertex(); glTexCoord2f(1, 0);
		Vector3(-0.5, 0.5, -0.5).glVertex(); glTexCoord2f(1, 1);
		Vector3(-0.5, 0.5, 0.5).glVertex(); glTexCoord2f(0, 1);
	}
	glEnd();

	// Right Face
	glBegin(GL_QUADS);
	{
		Vector3(0.5, -0.5, 0.5).glVertex(); glTexCoord2f(0, 0);
		Vector3(0.5, -0.5, -0.5).glVertex(); glTexCoord2f(1, 0);
		Vector3(0.5, 0.5, -0.5).glVertex(); glTexCoord2f(1, 1);
		Vector3(0.5, 0.5, 0.5).glVertex(); glTexCoord2f(0, 1);
	}
	glEnd();

	// Front Face
	glBegin(GL_QUADS);
	{
		Vector3(-0.5, -0.5, 0.5).glVertex(); glTexCoord2f(0, 0);
		Vector3(0.5, -0.5, 0.5).glVertex(); glTexCoord2f(1, 0);
		Vector3(0.5, 0.5, 0.5).glVertex(); glTexCoord2f(1, 1);
		Vector3(-0.5, 0.5, 0.5).glVertex(); glTexCoord2f(0, 1);
	}
	glEnd();

	// Back Face
	glBegin(GL_QUADS);
	{
		Vector3(-0.5, -0.5, -0.5).glVertex(); glTexCoord2f(0, 0);
		Vector3(0.5, -0.5, -0.5).glVertex(); glTexCoord2f(1, 0);
		Vector3(0.5, 0.5, -0.5).glVertex(); glTexCoord2f(1, 1);
		Vector3(-0.5, 0.5, -0.5).glVertex(); glTexCoord2f(0, 1);
	}
	glEnd();
};

// Cone
static const Mesh standardCone = []() { glutSolidCone(0.5, 1, 20, 20); };
static const Mesh standardWireCone = []() { glutWireCone(0.5, 1, 20, 20); };

// Torus
static const Mesh standardTorus = []() { glutSolidTorus(0.25, 0.5, 20, 20); };
static const Mesh standardWireTorus = []() { glutWireTorus(0.25, 0.5, 20, 20); };

// Dodecahedron
static const Mesh standardDodecahedron = []() { glutSolidDodecahedron(); };
static const Mesh standardWireDodecahedron = []() { glutWireDodecahedron(); };

// Octahedron
static const Mesh standardOctahedron = []() { glutSolidOctahedron(); };
static const Mesh standardWireOctahedron = []() { glutWireOctahedron(); };

// Tetrahedron
static const Mesh standardTetrahedron = []() {glutSolidTetrahedron(); };
static const Mesh standardWireTetrahedron = []() {glutWireTetrahedron(); };

// Icosahedron
static const Mesh standardIcosahedron = []() {glutSolidIcosahedron(); };
static const Mesh standardWireIcosahedron = []() {glutWireIcosahedron(); };

// Teapot
static const Mesh standardTeapot = []() {glutSolidTeapot(1); };
static const Mesh standardWireTeapot = []() {glutWireTeapot(1); };

// Line
static const Mesh standardLine = []()
{
	glBegin(GL_LINES);
	{
		Vector3(-0.5, 0, 0).glVertex();
		Vector3(0.5, 0, 0).glVertex();
	}
	glEnd();
};

// Plane
static const Mesh standardTexturePlane = []()
{
	glBegin(GL_QUADS);
	{
		Vector3(-0.5, 0, -0.5).glVertex(); glTexCoord2f(0, 0);
		Vector3(0.5, 0, -0.5).glVertex(); glTexCoord2f(1, 0);
		Vector3(0.5, 0, 0.5).glVertex(); glTexCoord2f(1, 1);
		Vector3(-0.5, 0, 0.5).glVertex(); glTexCoord2f(0, 1);
	}
	glEnd();
};