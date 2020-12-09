#include "SceneObject.h"

SceneObject::SceneObject()
{
	transformation = Transformation();
	material = Material();
	mesh = standardTextureCube;
}

SceneObject::SceneObject(const Mesh& mesh_)
{
	transformation = Transformation();
	material = Material();
	mesh = mesh_;
}

SceneObject::SceneObject(const SceneObject& rhs)
{
	transformation = rhs.transformation;
	material = rhs.material;
	mesh = rhs.mesh;
}

SceneObject& SceneObject::operator=(SceneObject rhs)
{
	swap(rhs);
	return *this;
}

void SceneObject::swap(SceneObject& rhs)
{
	std::swap(transformation, rhs.transformation);
	std::swap(material, rhs.material);
	std::swap(mesh, rhs.mesh);
}

void SceneObject::glDraw()
{
	glPushMatrix();
	{
		transformation.transform();
		material.materialize();
		mesh();
	}
	glPopMatrix();
}