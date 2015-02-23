
#include <iostream>

#include <math.h>

#include "vector2.hh"

Vector2::Vector2() {
  x = 0;
  y = 0;
}

Vector2::Vector2( float _x, float _y ) {
  x = _x;
  y = _y;
};

void Vector2::set( float _x, float _y ) {
  x = _x;
  y = _y;
}

void Vector2::set( const Vector2 &v ) {
  x = v.x;
  y = v.y;
}

void Vector2::set_as_angle( float a ) {
  x = cos(a);
  y = sin(a);
}

float Vector2::magnitude() const {
  return sqrt( x * x + y * y );
}

void Vector2::normalize() {
  float m = magnitude();

  x /= m;
  y /= m;
}

void Vector2::invert() {
  x = -x;
  y = -y;
}

void Vector2::operator*=( float q ) {

  x *= q;
  y *= q;
}

void Vector2::operator+=( float q ) {

  x += q;
  y += q;
}

void Vector2::operator*=( const Vector2 &o ) {
  x *= o.x;
  y *= o.y;
}

void Vector2::operator+=( const Vector2 &o ) {
  x += o.x;
  y += o.y;
}

float Vector2::dot( const Vector2 &v ) const {
  return x * v.x + y * v.y;
}

void Vector2::dump() const {
  std::cout << "x=" << x << ", y=" << y << std::endl;
}

Vector2 Vector2::operator-() const {
  return Vector2( -x, -y );
}

Vector2 Vector2::operator-( const Vector2 &v ) const {
  return Vector2( x - v.x, y - v.y );
}

Vector2 Vector2::operator+( const Vector2 &v ) const {
  return Vector2( x + v.x, y + v.y );
}

Vector2 Vector2::operator*( float d ) const {
  return Vector2( x * d, y * d );
}

Vector2 Vector2::perpendicular() const {
  return Vector2( -y, x );
}

Vector3 Vector2::to_vector3() const {
  return Vector3( x, 0, y );
}

Vector3 Vector2::to_vector3(float a) const {
  return Vector3( x, a, y ); 
}

