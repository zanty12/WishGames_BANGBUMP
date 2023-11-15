/*==============================================================================

    Title		: COLOR
    Description : ÉJÉâÅ[
    Date		:
    Author		:

--------------------------------------------------------------------------------

==============================================================================*/



#pragma once
#ifndef _COLOR_H_
#define _COLOR_H_


/*------------------------------------------------------------------------------
* includeêÈåæ
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* defineêÈåæ
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* externêÈåæ
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* prototypeêÈåæ
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
*	ëOï˚êÈåæ
------------------------------------------------------------------------------*/


/*------------------------------------------------------------------------------
* classêÈåæ
------------------------------------------------------------------------------*/
class Color {
public:
    float r = 0.0f, g = 0.0f, b = 0.0f, a = 0.0f;

    static Color Red;
    static Color Green;
    static Color Blue;
    static Color Cyan;
    static Color Magenta;
    static Color Yellow;
    static Color Black;
    static Color White;
    static Color Gray;



public:
    Color() {}
    Color(float r, float g, float b) : r(r), g(g), b(b), a(1.f) {}
    Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}

    Color operator+ ();
    Color operator- ();
    Color operator+ (Color v);
    Color operator- (Color v);
    float operator* (Color v);
    friend Color operator* (Color v, float f) { return Color(v.r * f, v.g * f, v.b * f, v.a * f); }
    friend Color operator/ (Color v, float f) { return Color(v.r / f, v.g / f, v.b / f, v.a / f); }
    friend Color operator* (float f, Color v) { return Color(f * v.r, f * v.g, f * v.b, f * v.a); }
    friend Color operator/ (float f, Color v) { return Color(f / v.r, f / v.g, f / v.b, f / v.a); }

    Color operator+= (Color v);
    Color operator-= (Color v);
    friend Color operator*= (Color v, float f) { return Color(v.r *= f, v.g *= f, v.b *= f, v.a *= f); }
    friend Color operator/= (Color v, float f) { return Color(v.r /= f, v.g /= f, v.b /= f, v.a /= f); }
    friend Color operator*= (float f, Color v) { return Color(f *= v.r, f *= v.g, f *= v.b, f *= v.a); }
    friend Color operator/= (float f, Color v) { return Color(f /= v.r, f /= v.g, f /= v.b, f /= v.a); }

    bool operator== (Color v);
    bool operator!= (Color v);
};

#endif
