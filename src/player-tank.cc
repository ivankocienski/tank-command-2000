
#include "player-tank.hh"
#include "camera.hh"

#include <cmath>

using namespace std;

static const float tank_length = 3.0;
static const float tank_width  = 2.0;
static const float tank_view_height = 1.2;

PlayerTank::PlayerTank() {
  m_heading = 0;
}

void PlayerTank::set_pos( float x, float y ) { 
  m_position.set( x, tank_view_height, y );
}

void PlayerTank::walk(float d) { 
  m_new_pos = m_position + Vector3( m_direction.x, 0, m_direction.z ) * d;
}

void PlayerTank::move( vector<MeshInstance> & wo ) {

  Vector3 fwd = m_direction * (tank_length / 2.0);
  Vector3 rgt = m_right     * (tank_width  / 2.0);


  for( vector<MeshInstance>::iterator it = wo.begin(); it != wo.end(); it++ ) {

    Vector3 front_left = m_new_pos + fwd - rgt; 
    if( it->point_inside_bb( front_left )) return;

    Vector3 front_right = m_new_pos + fwd + rgt;
    if( it->point_inside_bb( front_right )) return;

    Vector3 back_left = m_new_pos - fwd - rgt;
    if( it->point_inside_bb( back_left )) return;

    Vector3 back_right = m_new_pos - fwd + rgt;
    if( it->point_inside_bb( back_right)) return;
  } 
  
  m_position = m_new_pos;
}

void PlayerTank::strafe( float d ) {
  m_position += Vector3( -m_direction.z, 0, m_direction.x ) * d;
}

void PlayerTank::turn(float d) {
  m_heading += d;
  if( m_heading >= 2 * M_PI ) m_heading -= 2 * M_PI;
  if( m_heading < 0 ) m_heading += 2 * M_PI;

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

float PlayerTank::heading() {
  return m_heading;
}
