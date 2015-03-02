
#pragma once

#include <vector>

#include "obstacle.hh"
#include "baddies/mid-tank.hh"
#include "mesh-instance.hh"
#include "math/vector2.hh"

class Bullet {
private:

  MeshInstance m_mesh_instance;

  Vector2 m_position;
  Vector2 m_direction;

  float m_angle;
  int   m_alive;
  int   m_owner;

public:

  enum {
    B_PLAYER,
    B_ENEMY
  };
  
  Bullet();
  Bullet( const Vector2&, float, int, int );

  int owner();

  void move();
  bool is_active();

  bool has_hit_obstacle( std::vector<Obstacle>& );
  MidTank* has_hit_enemy( std::vector<MidTank>& );
  
  MeshInstance & mesh();

  const Vector2 & position();
};

