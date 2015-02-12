
#pragma once

#include "math/matrix4.hh"
#include "math/vector3.hh"
#include "mesh.hh"
#include "camera.hh"

class Player {
private:


  Vector3 m_position;
  
  Vector3 m_direction;
  Vector3 m_right;

  float m_heading;

public:

  Player();

  void look(Camera&);

  void walk(float);
  void turn(float);

  float foot_x();
  float foot_y();
};
