
#pragma once

#include <math.h>

#include "vector2.hh"

class Matrix3 {
public:

  float m_v[9];

  Matrix3();

  float & operator[](int);
  
  void identity();
  void set_rotation( float );
  void set_translation( const Vector2& );
  void invert();

  float determinant();

  void operator=( const Matrix3& );
  void operator*=( const Matrix3& );
};


