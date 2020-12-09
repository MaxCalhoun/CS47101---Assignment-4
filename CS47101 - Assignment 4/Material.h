#pragma once

#include "Color.h"

class Material
{
	public:
		// Member Variable
		Color color;

		// Constructors
		Material();
		Material(const Color&);
		Material(const Material&);

		// Destructor
		~Material() {};

		void materialize();
};
