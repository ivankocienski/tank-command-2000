
#pragma once

#include <vector>

#include "math/vector3.hh"
#include "mesh-instance.hh"

class Camera;

class PlayerTank {
private:

  Vector3 m_position;
  Vector3 m_new_pos;
  
  Vector3 m_direction;
  Vector3 m_right;

  float m_heading;
  
public:

  PlayerTank();

  void set_pos( float, float );

  void look(Camera*);

  void move( std::vector<MeshInstance> & );

  void walk(float);
  void turn(float);
  void strafe(float);

  float foot_x();
  float foot_y();
};

