
#pragma once

#include "mesh-instance.hh"
#include "math/vector2.hh"

class Powerup {
private:

  MeshInstance m_mesh_instance;
  Vector2 m_position;

  float m_angle;

  int m_type;
  int m_alive;

public:

  enum {
    PT_SCORE,
    PT_ARMOUR
  };

  Powerup();
  Powerup(const Vector2&, int);

  void update();
  int type();
  bool is_active();

  const Vector2 & position();

  MeshInstance & mesh_instance();
};

