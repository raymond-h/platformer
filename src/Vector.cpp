#include "Vector.h"

#include <cmath>
#include <iostream>

const float Vector::PI = 3.14159265f;

Vector Vector::fromPolar(float angle, float distance) {
	return Vector( std::cos(angle)*distance, std::sin(angle)*distance );
}

Vector Vector::operator+(const Vector& rhs) {
	return Vector( x_ + rhs.x_, y_ + rhs.y_ );
}

Vector Vector::operator-(const Vector& rhs) {
	return Vector( x_ - rhs.x_, y_ - rhs.y_ );
}

Vector Vector::operator/(const Vector& rhs) {
	return Vector( x_ / rhs.x_, y_ / rhs.y_ );
}

Vector Vector::operator*(const Vector& rhs) {
	return Vector( x_ * rhs.x_, y_ * rhs.y_ );
}

Vector Vector::operator/(float scalar) {
	return Vector( x_ / scalar, y_ / scalar );
}

Vector Vector::operator*(float scalar) {
	return Vector( x_ * scalar, y_ * scalar );
}

Vector& Vector::operator+=(const Vector& rhs) {
	x_+=rhs.x_; y_+=rhs.y_;
	return *this;
}

Vector& Vector::operator-=(const Vector& rhs) {
	x_-=rhs.x_; y_-=rhs.y_;
	return *this;
}

Vector& Vector::operator/=(const Vector& rhs) {
	x_/=rhs.x_; y_/=rhs.y_;
	return *this;
}

Vector& Vector::operator*=(const Vector& rhs) {
	x_*=rhs.x_; y_*=rhs.y_;
	return *this;
}

Vector& Vector::operator/=(float scalar) {
	x_/=scalar; y_/=scalar;
	return *this;
}

Vector& Vector::operator*=(float scalar) {
	x_*=scalar; y_*=scalar;
	return *this;
}

const float Vector::length() const {
	return std::sqrt( lengthSquared() );
}

Vector Vector::normalize() {
	float len = length();
	return Vector( x_/len, y_/len );
}

const float Vector::angle() const {
	return std::atan2(y_, x_);
}

void Vector::angle(float a) {
	float d = length();
	x_ = std::cos(a)*d;
	y_ = std::sin(a)*d;
}

void Vector::distance(float d) {
	d /= length();
	operator*=(d);
}