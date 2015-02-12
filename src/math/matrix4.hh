
#pragma once

//somehow this will be used...

#include "vector3.hh"

class Matrix4 {
public:
  float m_v[16];

  Matrix4();

  void identity();
  Matrix4 operator*( const Matrix4& );
  
  void set_translate( Vector3& );
  void set_rotation( Vector3& );

  float & operator[](int);
};
