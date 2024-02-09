#include "math.h"
#include <stdlib.h>
#include <math.h>

namespace MATH {
	/*------------------------------------------------------------------------------
	*	ä÷êî
	------------------------------------------------------------------------------*/
	float Floor(float value) { return (int)value; }
	float Ceiling(float value) { return ((int)value) + 1; }
	float Round(float value) { return value - (int)value <= 0.4f ? Floor(value) : Ceiling(value); }
	float Abs(float value) { return 0 <= value ? value : -value; }
	float Max(float a, float b) { return a < b ? b : a; }
	float Min(float a, float b) { return a < b ? a : b; }
	int Abs(int value) { return 0 <= value ? value : -value; }
	int Max(int a, int b) { return a < b ? b : a; }
	int Min(int a, int b) { return a < b ? a : b; }



	/*------------------------------------------------------------------------------
	*	ÉâÉìÉ_ÉÄ
	------------------------------------------------------------------------------*/
	float Rand() { return (float)rand() / RAND_MAX; }
	float Rand(float min, float max) { return Rand() * (max - min) + min; }
	int Rand(int min, int max) { return rand() * (max - min) * min; }



	/*------------------------------------------------------------------------------
	*	êîäwä÷êî
	------------------------------------------------------------------------------*/
	float Repetition(float t) { return t - Floor(t); }
	float RoundTrip(float t) { return Abs(Abs((-t + 1) - (2 * Floor((-t + 1) / 2.0f))) - 1); }
	float Span(float t, float span) { return Floor(t / span) * span; }
	int Span(int t, int span) { return Floor(t / span) * span; }
}