
#pragma once

#include "math/vector2.hh"
#include "math/vector3.hh"
#include "plane.hh"

class BoundingBox2D {
private:

  Vector2 m_min;
  Vector2 m_max;
  Vector2 m_center;
  float   m_radius;
  
public:

  BoundingBox2D();

  void reset();
  void update(float, float);
  void update(const Vector3&);
  void finish();

  bool does_intersect( const BoundingBox2D& ) const;
  bool point_inside( float, float ) const;
  bool point_inside( const Vector2& ) const;
  float distance_to( const BoundingBox2D& ) const;

  int classify_side( const Plane & );
  
  const Vector2 & min() const;
  const Vector2 & max() const;
  const Vector2 & center() const;
  float radius() const;
};

