
#include <cstring>

#include "matrix3.hh"

#define M11 m_v[0]
#define M12 m_v[1]
#define M13 m_v[2]

#define M21 m_v[3]
#define M22 m_v[4]
#define M23 m_v[5]

#define M31 m_v[6]
#define M32 m_v[7]
#define M33 m_v[8]

Matrix3::Matrix3() {
  memset( m_v, 0, sizeof(m_v) );
}

void Matrix3::identity() {
  memset( m_v, 0, sizeof(m_v));

  M11 = 1;
  M22 = 1;
  M33 = 1;
}

void Matrix3::set_rotation( float d ) {

  float c = cos(d);
  float s = sin(d);

  // this is actually a 2D rotation

  M11 = c;
  M12 = s;
  M21 = -s;
  M22 = c;
  M33 = 1.0;
}

void Matrix3::set_translation( const Vector2 &v ) {
  
  M13 = v.x;
  M23 = v.y;
  M11 = 1;
  M22 = 1;
  M33 = 1;
}

float & Matrix3::operator[]( int i ) {
  return m_v[i];
}

void Matrix3::operator=( const Matrix3 &o ) {
  memcpy( m_v, o.m_v, sizeof(m_v));
}

void Matrix3::operator*=( const Matrix3 &o ) {
  
  float r[9];

  r[0] = M11 * o.M11   +   M12 * o.M12   +   M13 * o.M13;
  r[1] = M11 * o.M21   +   M12 * o.M22   +   M13 * o.M23;
  r[2] = M11 * o.M31   +   M12 * o.M32   +   M13 * o.M33;
  
  r[3] = M21 * o.M11   +   M22 * o.M12   +   M23 * o.M13;
  r[4] = M21 * o.M21   +   M22 * o.M22   +   M23 * o.M23;
  r[5] = M21 * o.M31   +   M22 * o.M32   +   M23 * o.M33;

  r[6] = M31 * o.M11   +   M32 * o.M12   +   M33 * o.M13;
  r[7] = M31 * o.M21   +   M32 * o.M22   +   M33 * o.M23;
  r[8] = M31 * o.M31   +   M32 * o.M32   +   M33 * o.M33;

  memcpy( m_v, r, sizeof(m_v));
}

float Matrix3::determinant() {
  float d;

  d  = M11 * M22 * M33;
  d += M12 * M23 * M31;
  d += M13 * M21 * M32;

  d -= M13 * M22 * M31;
  d -= M12 * M21 * M33;
  d -= M11 * M23 * M32;

  return d;
}

#define DET2( a, b, c, d ) (a * d - b * c)

void Matrix3::invert() {

  float c11 =  DET2( M22, M23, M32, M33 );
  float c12 = -DET2( M21, M23, M31, M33 );
  float c13 =  DET2( M21, M22, M31, M32 );

  float c21 = -DET2( M12, M13, M32, M33 );
  float c22 =  DET2( M11, M13, M31, M33 );
  float c23 = -DET2( M11, M12, M31, M32 );

  float c31 =  DET2( M12, M13, M22, M23 );
  float c32 = -DET2( M11, M13, M21, M23 );
  float c33 =  DET2( M11, M12, M21, M22 );

  float d = 1.0 / determinant();

  m_v[0] = c11 * d;
  m_v[1] = c12 * d;
  m_v[2] = c13 * d;
  
  m_v[3] = c21 * d;
  m_v[4] = c22 * d;
  m_v[5] = c23 * d;
  
  m_v[6] = c31 * d;
  m_v[7] = c32 * d;
  m_v[8] = c33 * d;
}

