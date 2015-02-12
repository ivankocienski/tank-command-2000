
#include <cmath>

#include "player.hh"

Player::Player() {
  m_heading = 0;
  turn(0);
}

void Player::walk(float d) {
  /*
  there's a bit of a mess here. our heading uses simple cos/sin functions
  to give the direction vector the proper values of unit length. But the
  camera translation function also uses this vector as if cos and sin where
  set up properly for it.
  The two 'concepts' must conflict; at zero degrees the direction in player
  points right (+x axis) but in the camera zero degrees points forward
  (+z axis).
  So we need to do some fududling here by 'rotating' the players direction
  by 90 degrees to be inline with the projection matrix.
  */

  m_position += Vector3( m_direction.x, 0, m_direction.z ) * d;
}

void Player::turn(float d) {
  m_heading += d;

  //m_direction.set( cos(m_heading), 0, sin(m_heading));
  
  
  //m_direction.set( sin(m_heading), 0, -cos(m_heading));


  // the long way ...
  m_direction.set( 1, 0, 0 );
  m_right.set( 0, 0, 1 );
  
  Matrix4 rot_mat;
  rot_mat.identity();

  float s = sin(m_heading);
  float c = cos(m_heading);

  rot_mat.m_v[0]  =  c;
  rot_mat.m_v[2]  = -s;
  rot_mat.m_v[8]  =  s;
  rot_mat.m_v[10] =  c;


  {
    float nx = m_direction.x * rot_mat[0] + m_direction.y * rot_mat[1] + m_direction.z * rot_mat[2]  + rot_mat[3];
    float ny = m_direction.x * rot_mat[4] + m_direction.y * rot_mat[5] + m_direction.z * rot_mat[6]  + rot_mat[7];
    float nz = m_direction.x * rot_mat[8] + m_direction.y * rot_mat[9] + m_direction.z * rot_mat[10] + rot_mat[11];

    m_direction.set(nx, ny, nz);
  }


  {
    float nx = m_right.x * rot_mat[0] + m_right.y * rot_mat[1] + m_right.z * rot_mat[2]  + rot_mat[3];
    float ny = m_right.x * rot_mat[4] + m_right.y * rot_mat[5] + m_right.z * rot_mat[6]  + rot_mat[7];
    float nz = m_right.x * rot_mat[8] + m_right.y * rot_mat[9] + m_right.z * rot_mat[10] + rot_mat[11];

    m_right.set(nx, ny, nz);
  }
}

float Player::foot_x() {
  return m_position.x;
}

float Player::foot_y() {
  return m_position.z;
}

void Player::look(Camera &cam ) {
  cam.look( m_position, m_direction, m_right );
}

