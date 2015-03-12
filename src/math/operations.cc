
#include "operations.hh"


void vec3_mat3_multiply( Vector3& v, const Matrix3& m ) {

  /* isn't there supposed to be some wickidly cool CPU instruction
   * that does all this? */
  float x = m.m_v[0] * v.x + m.m_v[1] * v.y + m.m_v[2] * v.z;
  float y = m.m_v[3] * v.x + m.m_v[4] * v.y + m.m_v[5] * v.z;
  float z = m.m_v[6] * v.x + m.m_v[7] * v.y + m.m_v[8] * v.z;

  v.x = x;
  v.y = y;
  v.z = z;
}

void vec3_mat4_multiply( Vector3& v, const Matrix4& m ) {

  /* isn't there supposed to be some wickidly cool CPU instruction
   * that does all this? */
  
  float x = m.m_v[0] * v.x + m.m_v[1] * v.y + m.m_v[ 2] * v.z + m.m_v[ 3];
  float y = m.m_v[4] * v.x + m.m_v[5] * v.y + m.m_v[ 6] * v.z + m.m_v[ 7];
  float z = m.m_v[8] * v.x + m.m_v[9] * v.y + m.m_v[10] * v.z + m.m_v[11];

  v.x = x;
  v.y = y;
  v.z = z;
}

void vec2_mat3_multiply( Vector2 &v, const Vector2 &vi, const Matrix3 &m ) {

  v.x = m.m_v[0] * vi.x + m.m_v[1] * vi.y + m.m_v[2] * 1.0;
  v.y = m.m_v[3] * vi.x + m.m_v[4] * vi.y + m.m_v[5] * 1.0;
}
