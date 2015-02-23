
#include "player-tank.hh"
#include "camera.hh"

#include <cmath>

using namespace std;

static const float tank_length = 3.0;
static const float tank_width  = 2.0;
static const float tank_view_height = 1.2;

PlayerTank::PlayerTank() {
  m_heading = 0;
  m_height  = tank_view_height;
}

void PlayerTank::set_pos( float x, float y ) { 
  m_position.set( x, y );
}

void PlayerTank::walk(float d) { 
  m_new_pos = m_position + m_direction * d;
}

Vector2 & PlayerTank::position() {
  return m_position;
}

void PlayerTank::move( vector<Obstacle> & wo ) {

  Vector2 fwd = m_direction * (tank_length / 2.0);
  Vector2 rgt = m_right     * (tank_width  / 2.0);

  Vector2 front_left  = m_new_pos + fwd - rgt; 
  Vector2 front_right = m_new_pos + fwd + rgt;
  Vector2 back_left   = m_new_pos - fwd - rgt;
  Vector2 back_right  = m_new_pos - fwd + rgt;

  
  for( vector<Obstacle>::iterator it = wo.begin(); it != wo.end(); it++ ) {

    MeshInstance &mi = it->mesh();
    
    if( mi.point_inside_bb( front_left ))  return;
    if( mi.point_inside_bb( front_right )) return;
    if( mi.point_inside_bb( back_left ))   return;
    if( mi.point_inside_bb( back_right))   return;
  } 
  
  m_position = m_new_pos;
}

void PlayerTank::strafe( float d ) {
  m_position += m_direction.perpendicular() * d;
}

void PlayerTank::turn(float d) {

  m_heading += d;
  if( m_heading >= 2 * M_PI ) m_heading -= 2 * M_PI;
  if( m_heading < 0 ) m_heading += 2 * M_PI;

  // the long way ...
  m_direction.set( 1, 0 );
  m_right.set( 0, 1 );
  
  Matrix4 rot_mat;
  rot_mat.identity();

  float s = sin(m_heading);
  float c = cos(m_heading);

  rot_mat.m_v[0]  =  c;
  rot_mat.m_v[2]  = -s;
  rot_mat.m_v[8]  =  s;
  rot_mat.m_v[10] =  c;

  // ugh. transform a 2x1 vector by a 4x4 matrix. ignore Z component. W is always 1

  {
    float nx = m_direction.x * rot_mat[0] + m_direction.y * rot_mat[2]  + rot_mat[3];
    float ny = m_direction.x * rot_mat[8] + m_direction.y * rot_mat[10] + rot_mat[11];

    m_direction.set(nx, ny );
  }

  {
    float nx = m_right.x * rot_mat[0] + m_right.y * rot_mat[2]  + rot_mat[3];
    float ny = m_right.x * rot_mat[8] + m_right.y * rot_mat[10] + rot_mat[11];

    m_right.set(nx, ny );
  }
}

void PlayerTank::look(Camera *cam ) {
  cam->look( 
      m_position.to_vector3(m_height),
      m_direction.to_vector3(),
      m_right.to_vector3() );
}

float PlayerTank::heading() {
  return m_heading;
}
