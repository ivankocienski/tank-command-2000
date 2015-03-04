
#pragma once

#include <vector>

#include "math/vector2.hh"
#include "mesh-instance.hh"
#include "obstacle.hh"

class Camera;
class World;

class PlayerTank {
private:

  World *m_world;

  Vector2 m_position;
  Vector2 m_new_pos;
  
  Vector2 m_direction;
  Vector2 m_right;

  float m_heading;
  float m_height;

  int m_armour;
  bool m_fire_now;
  
public:

  PlayerTank(World*);
  void reset();

  int armour();
  void do_damage(int);
  
  Vector2& position();
  void set_pos( float, float );

  void look(Camera*);
  float heading();

  bool is_hit_by( const Vector2& );
  
  void move( std::vector<Obstacle> & );
  void fire(bool);

  void walk(float);
  void turn(float);

  void strafe(float);
  void raise(float);
  void zero_y();
};

