
#pragma once

#include "math/vector3.hh"

class Plane {
private:

  Vector3 m_normal;
  Vector3 m_position;
  float   m_distance;

public:

  Plane();
    
  void setup( const Vector3&, const Vector3& );
  bool is_facing( const Vector3& ) const;

  const Vector3& position() const;
  const Vector3& normal() const;
};
