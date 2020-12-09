#pragma once

#include "Vector3.h"

class Transformation
{
	public:
		// Member Variables
		Vector3 position, rotation, scale;

		// Constructors
		Transformation();
		Transformation(const Vector3&, const Vector3&, const Vector3&);
		Transformation(const Transformation&);

		// Destructor
		~Transformation() {};

		// Assignement
		Transformation& operator=(Transformation);
		void swap(Transformation&);

		// Comparison
		bool operator==(const Transformation&) const;
		bool operator!=(const Transformation&) const;

		// IO
		//friend std::ostream& operator<<(std::ostream&, const Transformation&);
		//friend std::istream& operator>>(std::istream&, Transformation&);

		// Perform Transformation
		void transform();
};
