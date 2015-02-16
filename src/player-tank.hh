
#pragma once

#include "math/vector3.hh"

class Camera;

class PlayerTank {
private:

  Vector3 m_position;
  
  Vector3 m_direction;
  Vector3 m_right;

  float m_heading;
  
public:

  PlayerTank();

  void look(Camera*);

  void walk(float);
  void turn(float);
  void strafe(float);

  float foot_x();
  float foot_y();
};

