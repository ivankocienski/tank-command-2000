
#pragma once

#include "mesh-instance.hh"

class ExplodingPart {
private:

  MeshInstance m_mesh_instance;

  Vector2 m_position;
  Vector2 m_direction;
  float   m_height;
  float   m_speed;

  float m_rot_alpha;
  float m_rot_beta;

  float m_rot_alpha_inc;
  float m_rot_beta_inc;
    
  float m_theta;
  float m_theta_inc;

  int m_count;

public:
  
  ExplodingPart();
  void init( const Vector2& );
  
  MeshInstance & mesh_instance();
  bool active();
  void update();
};

