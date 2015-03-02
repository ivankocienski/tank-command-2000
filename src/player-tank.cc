
#include "player-tank.hh"
#include "camera.hh"
#include "world.hh"

#include <cmath>

using namespace std;

static const float tank_length = 3.0;
static const float tank_width  = 2.0;
static const float tank_view_height = 1.2;

PlayerTank::PlayerTank(World *w) {
  m_world   = w;
  m_heading = 0;
  m_height  = tank_view_height;
  m_armour  = 100;
}

int PlayerTank::armour() {
  return m_armour;
}

void PlayerTank::set_pos( float x, float y ) { 
  m_position.set( x, y );
  m_new_pos.set( x, y );
}

void PlayerTank::walk(float d) { 
  m_new_pos = m_position + m_direction * d;
}

void PlayerTank::strafe( float d ) {
  m_new_pos = m_position + m_direction.perpendicular() * d;
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

  
//  for( vector<Obstacle>::iterator it = wo.begin(); it != wo.end(); it++ ) {
//
//    MeshInstance &mi = it->mesh();
//    
//    if( mi.point_inside_bb( front_left ))  return;
//    if( mi.point_inside_bb( front_right )) return;
//    if( mi.point_inside_bb( back_left ))   return;
//    if( mi.point_inside_bb( back_right))   return;
//  } 
  
  m_position = m_new_pos;
}


void PlayerTank::turn(float d) {

  m_heading += d;
  if( m_heading >= 2 * M_PI ) m_heading -= 2 * M_PI;
  if( m_heading < 0 ) m_heading += 2 * M_PI;

  // the long way ...
  m_direction.set( 1, 0 );
  m_right.set( 0, 1 );
  
  float s = sin(m_heading);
  float c = cos(m_heading);

  // ugh. transform a 2x1 vector by a 4x4 matrix. ignore Y component. W is always 1

  {
    float nx = m_direction.x * c + m_direction.y * -s;
    float ny = m_direction.x * s + m_direction.y *  c;

    m_direction.set(nx, ny );
  }

  {
    float nx = m_right.x * c + m_right.y * -s;
    float ny = m_right.x * s + m_right.y *  c;

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

void PlayerTank::fire( bool d ) {

  if( d && !m_fire_now ) m_world->shoot_player_bullet( m_position, m_heading );

  m_fire_now = d; 
}

void PlayerTank::raise( float f) {
  m_height += f;
}

bool PlayerTank::is_hit_by( const Vector2& p ) {
  if( p.x < m_position.x - 0.5 ) return false;
  if( p.x > m_position.x + 0.5 ) return false;
  if( p.y < m_position.y - 0.5 ) return false;
  if( p.y > m_position.y + 0.5 ) return false;

  return true; 
}

void PlayerTank::zero_y() {
  m_height = tank_view_height;
}
