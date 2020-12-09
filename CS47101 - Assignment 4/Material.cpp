#include "Material.h"

Material::Material()
{
	color = Color();
}

Material::Material(const Color& c)
{
	color = c;
}

Material::Material(const Material& rhs)
{
	color = rhs.color;
}

void Material::materialize()
{
	float r = (float)color.getRed() / 255.0f;
	float g = (float)color.getGreen() / 255.0f;
	float b = (float)color.getBlue() / 255.0f;

	float rgb[4] = { r, g, b, 1 };

	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, rgb);
}