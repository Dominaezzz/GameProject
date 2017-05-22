#pragma once

struct Color
{
	float r, g, b, a;

	Color() = default;
	constexpr Color(float r, float g, float b, float a = 1.0f) : r(r), g(g), b(b), a(a) {}
	explicit constexpr Color(unsigned int rgba) :
	r(((rgba >>  0) & 0xFF) / 255.0F),
	g(((rgba >>  8) & 0xFF) / 255.0F),
	b(((rgba >> 16) & 0xFF) / 255.0F),
	a(((rgba >> 24) & 0xFF) / 255.0F) {}

	explicit operator unsigned int() const;

//	static constexpr Color fromFloats(float r, float g, float b, float a = 1.0f)
//	{
//		return Color(
//			static_cast<unsigned char>(r * 255),
//			static_cast<unsigned char>(g * 255),
//			static_cast<unsigned char>(b * 255),
//			static_cast<unsigned char>(a * 255)
//		);
//	}

	static const Color White;
	static const Color Black;
	static const Color Transparent;
	static const Color Red;
	static const Color Green;
	static const Color Blue;
};

inline Color::operator unsigned int() const
{
	return (static_cast<int>(a * 255) << 24) |
		(static_cast<int>(b * 255) << 16) |
		(static_cast<int>(g * 255) << 8) |
		(static_cast<int>(r * 255) << 0);
}
