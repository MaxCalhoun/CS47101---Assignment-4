#pragma once

#include <iostream>
#include <GL/glut.h>

class Color
{
	public:
		// Constructors
		Color();
		Color(int, int, int, int);
		Color(const Color&);

		// Destructor
		~Color() {};

		// Accessors
		int getRed() const;
		int getGreen() const;
		int getBlue() const;
		int getAphla() const;

		// Mutators
		void setRed(int);
		void setGreen(int);
		void setBlue(int);
		void setAlpha(int);

		// Assignment
		Color& operator=(Color);
		void swap(Color&);

		// Comparison
		bool operator==(const Color&) const;
		bool operator!=(const Color&) const;

		// Output
		friend std::ostream& operator<<(std::ostream&, const Color&);

		// Prebuilt Colors
		static Color red();
		static Color orange();
		static Color yellow();
		static Color green();
		static Color blue();
		static Color indego();
		static Color violet();
		static Color white();
		static Color black();
		static Color brown();

		void glColor() const;

	private:
		int red_, green_, blue_;
		int alpha_;
};
