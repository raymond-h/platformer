#include <initializer_list>

#ifndef VECTOR_H_
#define VECTOR_H_

class Vector {
public:
	Vector():x_(),y_() {};
	Vector(float x, float y):x_(x),y_(y) {};
	Vector(std::initializer_list<float> values):Vector(0.0f, 0.0f) {
		auto i = values.begin();
		x_ = *i;
		y_ = *(++i);
	};
	~Vector() {};

	static Vector fromPolar(float angle, float distance);

	Vector operator+(const Vector&);
	Vector operator-(const Vector&);
	Vector operator/(const Vector&);
	Vector operator*(const Vector&);

	Vector operator/(float);
	Vector operator*(float);

	Vector& operator+=(const Vector&);
	Vector& operator-=(const Vector&);
	Vector& operator/=(const Vector&);
	Vector& operator*=(const Vector&);

	Vector& operator/=(float);
	Vector& operator*=(float);

	const float length() const;
	const float lengthSquared() const { return x_*x_ + y_*y_; };

	Vector normalize();

	const float x() const { return x_; };
	const float y() const { return y_; };

	float& x() { return x_; };
	float& y() { return y_; };

	void x(float x) { x_ = x; };
	void y(float y) { y_ = y; };

	const float angle() const;
	const float distance() const { return length(); };

	void angle(float);
	void distance(float);

	void set_x(float x) { this->x(x); }
	void set_y(float y) { this->y(y); }
	void set_angle(float a) { angle(a); };
	void set_distance(float d) { distance(d); };

	float get_x() { return this->x(); };
	float get_y() { return this->y(); };
	float get_angle() { return angle(); };
	float get_distance() { return length(); };

	bool nonzero() { return x_ != 0 && y_ != 0; };

	static const float PI;

private:
	float x_;
	float y_;
};

#endif /* VECTOR_H_ */