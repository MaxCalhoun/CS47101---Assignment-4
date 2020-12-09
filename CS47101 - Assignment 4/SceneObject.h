#pragma once

#include "Transformation.h"
#include "Mesh.h"
#include "Material.h"

class SceneObject
{
	public:
		// Member Variables
		Transformation transformation;
		Material material;
		Mesh mesh;

		// Constructors
		SceneObject();
		SceneObject(const Mesh&);
		SceneObject(const SceneObject&);

		// Destructor
		~SceneObject() {};

		// Assignment
		SceneObject& operator=(SceneObject);
		void swap(SceneObject&);

		void glDraw();
};