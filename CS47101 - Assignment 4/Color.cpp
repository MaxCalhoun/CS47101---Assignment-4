#include "Color.h"

Color::Color()
{
	red_ = 255;
	green_ = 255;
	blue_ = 255;
	alpha_ = 255;
}

Color::Color(int r, int g, int b, int a)
{
	red_ = r % 256;
	green_ = g % 256;
	blue_ = b % 256;
	alpha_ = a % 256;
}

Color::Color(const Color& rhs)
{
	red_ = rhs.red_;
	green_ = rhs.green_;
	blue_ = rhs.blue_;
	alpha_ = rhs.alpha_;
}

int Color::getRed() const { return red_; }
int Color::getGreen() const { return green_; }
int Color::getBlue() const { return blue_; }
int Color::getAphla() const { return alpha_; }

void Color::setRed(int r) { red_ = r % 256; }
void Color::setGreen(int g) { green_ = g % 256; }
void Color::setBlue(int b) { blue_ = b % 256; }
void Color::setAlpha(int a) { alpha_ = a % 256; }

Color& Color::operator=(Color rhs)
{
	swap(rhs);
	return *this;
}

void Color::swap(Color& rhs)
{
	std::swap(red_, rhs.red_);
	std::swap(green_, rhs.green_);
	std::swap(blue_, rhs.blue_);
	std::swap(alpha_, rhs.alpha_);
}

bool Color::operator==(const Color& rhs) const
{
	bool rEqual = (red_ == rhs.red_);
	bool gEqual = (green_ == rhs.green_);
	bool bEqual = (blue_ == rhs.blue_);
	bool aEqual = (alpha_ == rhs.alpha_);

	return rEqual && gEqual && bEqual && aEqual;
}

bool Color::operator!=(const Color& rhs) const { return !(*this == rhs); }

std::ostream& operator<<(std::ostream& out, const Color& rhs)
{
	out << "<" << rhs.red_ << ", " << rhs.green_ << ", " << rhs.blue_ << ", " << rhs.alpha_ << ">";
	return out;
}

Color Color::red() { return Color(255, 0, 0, 255); }
Color Color::orange() { return Color(255, 128, 0, 255); }
Color Color::yellow() { return Color(255, 255, 0, 255); }
Color Color::green() { return Color(0, 255, 0, 255); }
Color Color::blue() { return Color(0, 0, 255, 255); }
Color Color::indego() { return Color(64, 0, 255, 255); }
Color Color::violet() { return Color(128, 0, 255, 255); }
Color Color::white() { return Color(255, 255, 255, 255); }
Color Color::black() { return Color(0, 0, 0, 255); }
Color Color::brown() { return Color(150, 75, 0, 255); }

void Color::glColor() const
{
	float r = red_ / 255.0f;
	float g = green_ / 255.0f;
	float b = blue_ / 255.0f;
	float a = alpha_ / 255.0f;

	glColor4f(r, g, b, a);
}