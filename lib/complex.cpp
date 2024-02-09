#include "complex.h"


Complex Complex::operator+() { return Complex(-re, -im); }
Complex Complex::operator-() { return Complex(-re, -im); }
Complex Complex::operator+(Complex c) { return Complex(re + c.re, im + c.im); }
Complex Complex::operator-(Complex c) { return Complex(re - c.re, im - c.im); }
Complex Complex::operator*(Complex c) { return Complex(re * c.re - im * c.im, re * c.im + c.re * im); }

Complex Complex::operator+=(Complex c) { return *this = Complex(re + c.re, im + c.im); }
Complex Complex::operator-=(Complex c) { return *this = Complex(re - c.re, im - c.im); }
Complex Complex::operator*=(Complex c) { return *this = Complex(re * c.re - im * c.im, re * c.im + c.re * im); }

Complex::operator Vector3() { return Vector3(re, im); }
Complex::operator Vector2() { return Vector2(re, im); }
