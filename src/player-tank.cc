
#include "math/operations.hh"

#include "player-tank.hh"
#include "camera.hh"
#include "world.hh"

#include <cmath>

using namespace std;

static const float tank_length = 3.0;
static const float tank_width  = 2.0;
static const float tank_view_height = 1.2;

static const float tank_max_turn_speed = 0.08;
static const float tank_acc_turn = 0.025;

static const float tank_max_move_speed = 0.2;
static const float tank_acc_move = 0.03;

PlayerTank::PlayerTank(World *w) {
  m_world = w;
  reset();
}

void PlayerTank::reset() {
  m_heading = 0;
  m_height  = tank_view_height;
  m_armour  = 100;

  m_want_walk  = 0;
  m_want_turn  = 0;
  m_walk_speed = 0;
  m_turn_speed = 0;
}

int PlayerTank::armour() {
  return m_armour;
}

void PlayerTank::do_damage(int d) {
  m_armour -= d;
  if(m_armour < 0) m_armour = 0;
}

void PlayerTank::set_pos( float x, float y ) { 
  m_position.set( x, y );
}

void PlayerTank::want_walk(float d) { 
  m_want_walk = d * tank_max_move_speed;
}

void PlayerTank::want_turn(float d) {
  m_want_turn = d * tank_max_turn_speed;
}

Vector2 & PlayerTank::position() {
  return m_position;
}

Matrix3 & PlayerTank::inv_model_matrix() {
  return m_inv_model;
}

static float sign_clamp( float val, float max ) {

  if( val < 0 ) if( val < -max ) return -max;
  if( val > max ) return max;

  return val;
}

void PlayerTank::move( vector<Obstacle> & wo ) {

  float walk_diff = m_want_walk - m_walk_speed;
  m_walk_speed += sign_clamp( walk_diff, tank_acc_move );

  float turn_diff = m_want_turn - m_turn_speed;
  m_turn_speed += sign_clamp( turn_diff, tank_acc_turn );


  if( m_turn_speed != 0 ) { // turning

    m_heading += m_turn_speed;

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

  if( m_walk_speed != 0 ) { // moving

    Vector2 new_pos = m_position + m_direction * m_walk_speed;

    Vector2 fwd = m_direction * (tank_length * 0.5);
    Vector2 rgt = m_right     * (tank_width  * 0.5);

    Vector2 front_left  = new_pos + fwd - rgt; 
    Vector2 front_right = new_pos + fwd + rgt;
    Vector2 back_left   = new_pos - fwd - rgt;
    Vector2 back_right  = new_pos - fwd + rgt;



    //  for( vector<Obstacle>::iterator it = wo.begin(); it != wo.end(); it++ ) {
    //
    //    const BoundingBox2 &bb = it->mesh()->bounds();
    //     
    //    if( bb.point_inside( front_left ))  return;
    //    if( bb.point_inside( front_right )) return;
    //    if( bb.point_inside( back_left ))   return;
    //    if( bb.point_inside( back_right))   return;
    //  } 

    m_position = new_pos; 
  } 

  m_inv_model.identity();

  {
    Matrix3 t;
    t.set_rotation( M_PI - m_heading );
    m_inv_model *= t; 
  }

  {
    Matrix3 t;
    t.set_translation( m_position );
    m_inv_model *= t;
  }

  
  m_inv_model.invert();

  m_want_turn = 0;
  m_want_walk = 0;
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

bool PlayerTank::is_touching( const Vector2& p ) {

  Vector2 t;
  
  vec2_mat3_multiply( t, p, m_inv_model );
  
  if( t.x < -(tank_width  * 0.5) ) return false;
  if( t.x >  (tank_width  * 0.5) ) return false;
  if( t.y < -(tank_length * 0.5) ) return false;
  if( t.y >  (tank_length * 0.5) ) return false;

  return true; 
}

