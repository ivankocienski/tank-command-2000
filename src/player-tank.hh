
#pragma once

#include <vector>

#include "math/vector2.hh"
#include "mesh-instance.hh"
#include "obstacle.hh"

class Camera;

class PlayerTank {
private:

  Vector2 m_position;
  Vector2 m_new_pos;
  
  Vector2 m_direction;
  Vector2 m_right;

  float m_heading;
  float m_height;
  
public:

  PlayerTank();

  Vector2& position();
  void set_pos( float, float );

  void look(Camera*);
  float heading();

  void move( std::vector<Obstacle> & );

  void walk(float);
  void turn(float);
  void strafe(float);
};

