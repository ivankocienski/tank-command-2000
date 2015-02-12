
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

/* void vec4_mat4_multiply( Vector4& v, const Matrix4& m ) {
 * 
 *   /..* isn't there supposed to be some wickidly cool CPU instruction
 *    * that does all this? *../
 *   float x = m.m_v[0] * v.x + m.m_v[1] * v.y + m.m_v[2] * v.z;
 *   float y = m.m_v[3] * v.x + m.m_v[4] * v.y + m.m_v[5] * v.z;
 *   float z = m.m_v[6] * v.x + m.m_v[7] * v.y + m.m_v[8] * v.z;
 * 
 *   v.x = x;
 *   v.y = y;
 *   v.z = z;
 * }
 */
