
#include <cmath>
#include <cstring>

#include "matrix4.hh"

Matrix4::Matrix4() {
  memset( m_v, 0, sizeof(m_v) );
}

void Matrix4::identity() {
  m_v[ 0] = 1;
  m_v[ 5] = 1;
  m_v[10] = 1;
  m_v[15] = 1;
}

float & Matrix4::operator[]( int i ) {
  return m_v[i];
}

#define C_11 0
#define C_12 1
#define C_13 2
#define C_14 3

#define C_21 4
#define C_22 5
#define C_23 6
#define C_24 7

#define C_31 8
#define C_32 9
#define C_33 10
#define C_34 11

#define C_41 12
#define C_42 13
#define C_43 14
#define C_44 15

#define M( a, b ) (m_v[a] * o.m_v[b])

Matrix4 Matrix4::operator*( const Matrix4 &o ) {

  Matrix4 r;

  r.m_v[C_11] = M( C_11, C_11 ) + M( C_12, C_21 ) + M( C_13, C_31 ) + M( C_14, C_41 );
  r.m_v[C_12] = M( C_11, C_12 ) + M( C_12, C_22 ) + M( C_13, C_32 ) + M( C_14, C_42 );
  r.m_v[C_13] = M( C_11, C_13 ) + M( C_12, C_23 ) + M( C_13, C_33 ) + M( C_14, C_43 );
  r.m_v[C_14] = M( C_11, C_14 ) + M( C_12, C_24 ) + M( C_13, C_34 ) + M( C_14, C_44 );

  r.m_v[C_21] = M( C_21, C_11 ) + M( C_22, C_21 ) + M( C_23, C_31 ) + M( C_24, C_41 );
  r.m_v[C_22] = M( C_21, C_12 ) + M( C_22, C_22 ) + M( C_23, C_32 ) + M( C_24, C_42 );
  r.m_v[C_23] = M( C_21, C_13 ) + M( C_22, C_23 ) + M( C_23, C_33 ) + M( C_24, C_43 );
  r.m_v[C_24] = M( C_21, C_14 ) + M( C_22, C_24 ) + M( C_23, C_34 ) + M( C_24, C_44 );

  r.m_v[C_31] = M( C_31, C_11 ) + M( C_32, C_21 ) + M( C_33, C_31 ) + M( C_34, C_41 );
  r.m_v[C_32] = M( C_31, C_12 ) + M( C_32, C_22 ) + M( C_33, C_32 ) + M( C_34, C_42 );
  r.m_v[C_33] = M( C_31, C_13 ) + M( C_32, C_23 ) + M( C_33, C_33 ) + M( C_34, C_43 );
  r.m_v[C_34] = M( C_31, C_14 ) + M( C_32, C_24 ) + M( C_33, C_34 ) + M( C_34, C_44 );

  r.m_v[C_41] = M( C_41, C_11 ) + M( C_42, C_21 ) + M( C_43, C_31 ) + M( C_44, C_41 );
  r.m_v[C_42] = M( C_41, C_12 ) + M( C_42, C_22 ) + M( C_43, C_32 ) + M( C_44, C_42 );
  r.m_v[C_43] = M( C_41, C_13 ) + M( C_42, C_23 ) + M( C_43, C_33 ) + M( C_44, C_43 );
  r.m_v[C_44] = M( C_41, C_14 ) + M( C_42, C_24 ) + M( C_43, C_34 ) + M( C_44, C_44 );

/*   r.m_v[0] = m_v[0] * o.m_v[0]  +  m_v[1] * o.m_v[4]  +  m_v[2] * o.m_v[8]   +  m_v[3] * o.m_v[12];
 *   r.m_v[1] = m_v[0] * o.m_v[1]  +  m_v[1] * o.m_v[5]  +  m_v[2] * o.m_v[9]   +  m_v[3] * o.m_v[13];
 *   r.m_v[2] = m_v[0] * o.m_v[2]  +  m_v[1] * o.m_v[6]  +  m_v[2] * o.m_v[10]  +  m_v[3] * o.m_v[14];
 *   r.m_v[3] = m_v[0] * o.m_v[2]  +  m_v[1] * o.m_v[6]  +  m_v[2] * o.m_v[10]  +  m_v[3] * o.m_v[14];
 */

  
  return r;
}

void Matrix4::set_translate( Vector3 &v ) {

  m_v[0]  = 1;
  m_v[5]  = 1;
  m_v[10] = 1;
  m_v[15] = 1;
  
  m_v[3] = v.x;
  m_v[7] = v.y;
  m_v[11] = v.z;
}

void Matrix4::set_rotation( Vector3 &v ) {

  // x=heading, y=pitch, z=bank

  float ch = cos(v.x);
  float sh = sin(v.x);
  float cp = cos(v.y);
  float sp = sin(v.y);
  float cb = cos(v.z);
  float sb = sin(v.z);

  m_v[0] = ch * cb + sh * sp * sb;
  m_v[1] = -ch * sb + sh * sp * cb;
  m_v[2] = sh * cp;
  
  m_v[4] = sb * cp;
  m_v[5] = cb * cp;
  m_v[6] = -sp;

  m_v[8] = -sh * cb + ch * sp * sb;
  m_v[9] = sb * sh + ch * sp * cb;
  m_v[10] = ch * cp;

  m_v[15] = 1;
}
