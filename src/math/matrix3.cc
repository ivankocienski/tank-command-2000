
#include <cstring>

#include "matrix3.hh"

Matrix3::Matrix3() {
  memset( m_v, 0, sizeof(m_v) );
}

float & Matrix3::operator[]( int i ) {
  return m_v[i];
}

