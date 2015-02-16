
#include "player-tank.hh"
#include "camera.hh"

#include <cmath>

PlayerTank::PlayerTank() {
}

void PlayerTank::walk(float d) {
  m_position += Vector3( m_direction.x, 0, m_direction.z ) * d;
}

void PlayerTank::strafe( float d ) {
  m_position += Vector3( -m_direction.z, 0, m_direction.x ) * d;
}

void PlayerTank::turn(float d) {
  m_heading += d;

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


float PlayerTank::foot_x() {
  return m_position.x;
}

float PlayerTank::foot_y() {
  return m_position.z;
}

void PlayerTank::look(Camera *cam ) {
  cam->look( m_position, m_direction, m_right );
}

