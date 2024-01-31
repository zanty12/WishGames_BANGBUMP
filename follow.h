#pragma once

class Follow {
private:
	float value_ = 0.0f;
	float dest_ = 0.0f;

public:
	float rate = 0.25f;

	Follow() = default;
	Follow(float value_) : value_(value_), dest_(value_) { }

	float operator =(float value_) { return (dest_ = value_); }
	operator float() { return value_; }
	void update(void) { value_ += get_tilt(); }
	float get_tilt(void) { return (dest_ - value_) * rate; }
	float get_value(void) { return value_; }
};