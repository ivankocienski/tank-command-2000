
#pragma once

#include "math/vector2.hh"
#include "mesh-instance.hh"

class Obstacle {
private:

  MeshInstance m_mesh;
  
  Vector2 m_position;
  float m_angle;
  float m_height;

public:

  Obstacle();
  Obstacle(int);

  void set_pos( float, float, float, float );

  Vector2 & position();

  MeshInstance & mesh();
};

