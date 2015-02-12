
#pragma once

//somehow this will be used...

class Matrix4 {
public:
  float m_v[16];

  Matrix4();

  void identity();
  Matrix4 operator*( const Matrix4& );

  float & operator[](int);
};
