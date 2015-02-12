
#pragma once

#include <math.h>

class Matrix3 {
public:

  float m_v[9];

  Matrix3();

  float & operator[](int);
  
  static Matrix3 identity() {
    Matrix3 mat;

    mat.m_v[0] = 1;
    mat.m_v[4] = 1;
    mat.m_v[8] = 1;

    return mat;
  }

  static Matrix3 rotation_x_matrix( Matrix3 &mat, float d ) {

    float c = cos(d);
    float s = sin(d);

    mat.m_v[0] = c;
    mat.m_v[2] = s;
    mat.m_v[4] = 1;
    mat.m_v[6] = -s;
    mat.m_v[8] = c;

    return mat;
  }

  static Matrix3 rotation_x_matrix( float d ) {
    Matrix3 mat;

    rotation_x_matrix( mat, d );

    return mat;
  }

};


