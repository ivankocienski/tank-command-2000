
#include <iostream>

#include <math.h>

#include "vector3.hh"

Vector3::Vector3() {
  x = 0;
  y = 0;
  z = 0;
}

Vector3::Vector3( float _x, float _y, float _z ) {
  x = _x;
  y = _y;
  z = _z;
};

void Vector3::set( float _x, float _y, float _z ) {
  x = _x;
  y = _y;
  z = _z;
}

void Vector3::set( const Vector3 &v ) {
  x = v.x;
  y = v.y;
  z = v.z; 
}

void Vector3::set_as_angle( float a ) {
  x = cos(a);
  y = 0;
  z = sin(a);
}

float Vector3::magnitude() const {
  return sqrt( x * x + y * y + z * z );
}

void Vector3::normalize() {
  float m = magnitude();

  x /= m;
  y /= m;
  z /= m; 
}

void Vector3::invert() {
  x = -x;
  y = -y;
  z = -z;
}

void Vector3::operator*=( float q ) {

  x *= q;
  y *= q;
  z *= q; 
}

void Vector3::operator+=( float q ) {

  x += q;
  y += q;
  z += q; 
}

void Vector3::operator*=( const Vector3 &o ) {
  x *= o.x;
  y *= o.y;
  z *= o.z; 
}

void Vector3::operator+=( const Vector3 &o ) {
  x += o.x;
  y += o.y;
  z += o.z; 
}

float Vector3::dot( const Vector3 &v ) const {
  return x * v.x + y * v.y + z * v.z;
}

Vector3 Vector3::cross( const Vector3 &v ) const {

  Vector3 nv;

  nv.x = y * v.z - z * v.y;
  nv.y = z * v.x - x * v.z;
  nv.z = x * v.y - y * v.x;

  return nv;
}

void Vector3::dump() const {
  std::cout << "x=" << x << ", y=" << y << ", z=" << z << std::endl;
}

Vector3 Vector3::operator-() const {
  return Vector3( -x, -y, -z );
}

Vector3 Vector3::operator-( const Vector3 &v ) const {
  return Vector3( x - v.x, y - v.y, z - v.z );
}

Vector3 Vector3::operator+( const Vector3 &v ) const {
  return Vector3( x + v.x, y + v.y, z + v.z );
}

Vector3 Vector3::operator*( float d ) const {
  return Vector3( x * d, y * d, z * d );
}

Vector3 Vector3::perpendicular() const {

  // sort of a cross product with an 'up' vector

  return Vector3( -z, 0, x );
  
}
