
#pragma once

#include "math/vector2.hh"

class BoundingBox2D {
private:

  Vector2 m_min;
  Vector2 m_max;

public:

  BoundingBox2D();

  void reset();
  void update(float, float);

  bool does_intersect( const BoundingBox2D& );
  bool point_inside( float, float );
  float distance_to( const BoundingBox2D& );

  const Vector2 & min() const;
  const Vector2 & max() const;
};

