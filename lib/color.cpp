#include "color.h"

Color Color::Red			= Color(1.0f, 0.0f, 0.0f);
Color Color::Green			= Color(0.0f, 1.0f, 0.0f);
Color Color::Blue			= Color(0.0f, 0.0f, 1.0f);
Color Color::Cyan			= Color(0.0f, 1.0f, 1.0f);
Color Color::Magenta		= Color(1.0f, 0.0f, 1.0f);
Color Color::Yellow			= Color(1.0f, 1.0f, 0.0f);
Color Color::Black			= Color(0.0f, 0.0f, 0.0f);
Color Color::White			= Color(1.0f, 1.0f, 1.0f);
Color Color::Gray			= Color(0.5f, 0.5f, 0.5f);



Color Color::operator+ () { return Color(+r, +g, +b, +a); }
Color Color::operator- () { return Color(-r, -g, -b, -a); }
Color Color::operator+ (Color v) { return Color(r + v.r, g + v.g, b + v.b, a + v.a); }
Color Color::operator- (Color v) { return Color(r - v.r, g - v.g, b - v.b, a - v.a); }
float Color::operator* (Color v) { return r * v.r + g * v.g; }
Color Color::operator+= (Color v) { return Color(r += v.r, g += v.g, b += v.b, a += v.a); }
Color Color::operator-= (Color v) { return Color(r -= v.r, g -= v.g, b -= v.b, a -= v.a); }
bool Color::operator== (Color v) { return r == v.r && g == v.g && b == v.b && a == v.a; }
bool Color::operator!= (Color v) { return r != v.r || g != v.g || b != v.b || a != v.a; }
