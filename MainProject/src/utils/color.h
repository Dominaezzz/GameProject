#pragma once

struct Color
{
	unsigned char r, g, b, a;

	Color() = default;
	constexpr Color(unsigned char r, unsigned char g, unsigned char b,  unsigned char a = 255) : r(r), g(g), b(b), a(a) {}

	explicit constexpr Color(unsigned int rgba) :
	r(static_cast<unsigned char>((rgba >>  0) & 0xFF)),
	g(static_cast<unsigned char>((rgba >>  8) & 0xFF)),
	b(static_cast<unsigned char>((rgba >> 16) & 0xFF)),
	a(static_cast<unsigned char>((rgba >> 24) & 0xFF)) {}

	explicit operator unsigned int() const;

	static const Color White;
};

inline Color::operator unsigned int() const
{
	return (a << 24) | (b << 16) | (g << 8) | (r << 0);
}
