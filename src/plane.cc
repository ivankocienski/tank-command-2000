
#include <cmath>

#include <iostream>
using std::cout;
using std::endl;

#include "common.hh"
#include "plane.hh"

Plane::Plane() {
  m_distance = 0;
}

void Plane::setup( const Vector3 &p, const Vector3 &d ) {

  m_normal   = d;
  m_position = p;
  m_distance = p.dot(m_normal);

}

bool Plane::is_facing( const Vector3& p ) const {
  
  float d = p.dot(m_normal);

  return (d - m_distance) > 0;
}

const Vector3& Plane::position() const {
  return m_position;
}

const Vector3& Plane::normal() const {
  return m_normal;
}
