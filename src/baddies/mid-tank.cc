
#include <iostream>

#include <math.h>

#include "mid-tank.hh"
#include "../assets.hh"
#include "../world.hh"

using namespace std;

static const float NEAR_MAX = 1000000;

/* TankMetric
 *
 * Compute and store various metrics for the tanks behaviour modules
 *
 */

TankMetric::TankMetric( MidTank *me, PlayerTank *tgt, std::vector<Obstacle>& obs ) {

  m_obs_distance = NEAR_MAX;
  m_obs_angle    = 0;
  m_obs_side     = 0;

  m_tgt_distance = NEAR_MAX;
  m_tgt_angle    = 0;
  m_tgt_side     = 0;

  const Vector2 & me_pos    = me->position();
  const Vector2 & me_dir    = me->direction();
  const Vector2 me_dir_perp = me_dir.perpendicular();

  // target

  Vector2 tgt_vector = tgt->position() - me_pos;

  m_tgt_distance = tgt_vector.magnitude();
  tgt_vector.normalize();
  m_tgt_angle    = me_dir.dot(tgt_vector); 
  m_tgt_side     = tgt_vector.dot(me_dir_perp);

//  cout << "tgt_distance=" << m_tgt_distance << endl;
  //cout << "tgt_angle=" << m_tgt_angle << endl;
  //cout << "tgt_side=" << m_tgt_side << endl;

  // obstacles

  float near = NEAR_MAX;
  Vector2 near_vto;

  vector<Obstacle>::iterator it;
  for( it = obs.begin(); it != obs.end(); it++ ) {
    
    // convert this over to
    // AABB intersection tests.

    Vector2 vector = it->position() - me_pos;
    float dist = vector.magnitude();// - it->mesh().bounds().radius();

    if( dist >= near ) continue;

    near = dist;
    near_vto.set( vector );
  }

  if( near < NEAR_MAX) {
    near_vto.normalize();
    m_obs_side     = near_vto.dot(me_dir_perp);
    m_obs_vector   = near_vto;
    m_obs_angle    = me_dir.dot(near_vto);
    m_obs_distance = near;
  }
}

float TankMetric::obstacle_distance()   { return m_obs_distance; } 
float TankMetric::obstacle_angle()      { return m_obs_angle; } 
float TankMetric::obstacle_side()       { return m_obs_side; } 
Vector2 & TankMetric::obstacle_vector() { return m_obs_vector; }

float TankMetric::target_angle()      { return m_tgt_angle; } 
float TankMetric::target_distance()   { return m_tgt_distance; } 
float TankMetric::target_side()       { return m_tgt_side; } 

/* TankFireControl
 *
 * Wraps the control of the tanks gun
 */

TankFireControl::T_FIRE_TABLE TankFireControl::s_fire_table [] = {
  /* distance, hold */
  {  0,  2, 500 },
  {  1,  5, 300 },
  {  2, 10, 200 },
  {  3, 20, 100 },
  { -1,  0,  0 }
};

TankFireControl::TankFireControl() {
  m_fire_entry = NULL;
  m_fire_hold  = 0;
}

void TankFireControl::scan( TankMetric& tm ) {
  
  m_is_locked_on = false;
  m_should_fire  = false;

  float dtt = tm.target_distance();

  if( !m_fire_entry ) {

    T_FIRE_TABLE * entry(s_fire_table);

    while( entry->pos >= 0 ) {

      if( dtt < entry->distance ) {
        entry++;
        continue;
      }

      break; 
    }

    if( entry->pos == -1 ) entry--;

    m_fire_entry = entry; 
  }

  // when in range- fire at them
  if( dtt <= m_fire_entry->distance ) {
    if( m_fire_hold ) {
      m_is_locked_on = true;
      m_should_fire  = (m_fire_hold % 30) == 0;
      m_fire_hold--;
      return;
    }
    m_fire_hold = m_fire_entry->hold;
  }

  // moving towards?
  if( dtt < m_fire_entry->distance ) {
    if( m_fire_entry->pos > 0 ) m_fire_entry--;
    m_fire_hold = m_fire_entry->hold;
  }

  // moving away?
  if( dtt > m_fire_entry->distance ) {
    if( m_fire_entry->pos < 3 ) m_fire_entry++;
    m_fire_hold = m_fire_entry->hold;
  }
}

bool TankFireControl::is_locked_on() { return m_is_locked_on; } 
bool TankFireControl::should_fire()  { return m_should_fire; }

/* TankObstacle
 *
 * Keep track of the obstacle we are trying to avoid
 */

TankObstacle::TankObstacle() {
  clear();
}

void TankObstacle::clear() {
  m_side = 0;
  m_mode = OM_INACTIVE;
}

bool TankObstacle::is_active() {
  return m_mode == OM_TURN || m_mode == OM_MOVE;
}

void TankObstacle::set( Vector2 &my_pos, TankMetric &tm ) {
  m_mark_point = my_pos;
  m_obs_vector = tm.obstacle_vector();
  m_side       = tm.obstacle_side();
  m_mode       = OM_TURN;
}

float TankObstacle::distance_to( Vector2& p ) {
  return (m_mark_point - p).magnitude();
}

float TankObstacle::angle_to( Vector2& d ) {
  return m_obs_vector.dot(d);
}

float TankObstacle::side() {
  return m_side;
}

int TankObstacle::mode() {
  return m_mode;
}

void TankObstacle::next_mode() {
  m_mode = (m_mode + 1) % OM_MAX;
}

/* MidTank
 * 
 * the main tank class
 */

MidTank::MidTank( ) {
  m_world   = NULL;
  m_heading = 0;
  m_height  = 0;
  m_active  = false;
}

MidTank::MidTank( World *w ) : m_mesh_instance( &g_mesh_list[ A_MID_TANK ]) {
  m_world   = w;
  m_heading = 0;
  m_height  = 0.5;
  m_active  = false;
  
  m_mesh_instance.set_color(1); // red
}

void MidTank::fire() {
  m_world->shoot_enemy_bullet( m_position + m_direction * 2.3, m_heading );
}

void MidTank::activate( const Vector2 &p ) {

  cout << "baddie activated" << endl;
  
  m_heading = 0;
  m_position.set(p);
  m_active = true;
  m_obstacle.clear();

  m_direction.set_as_angle( m_heading );

  m_mesh_instance.set_translation( m_position.to_vector3(m_height) );
  m_mesh_instance.set_rotation( M_PI - m_heading, 0, 0 );

  m_mesh_instance.transform();
}

bool MidTank::is_active() {
  return m_active;
}

void MidTank::deactivate() {
  cout << "baddie de-activated" << endl;
  m_active = false;
}

const Vector2 & MidTank::position() const {
  return m_position;
}

const Vector2 & MidTank::direction() const {
  return m_direction;
}

MeshInstance & MidTank::mesh_instance() {
  return m_mesh_instance;
}

void MidTank::think_and_move( PlayerTank *pt, vector<Obstacle> &obs ) {

  if( !m_active ) return;

  float h_inc = 0;
  Vector2 p_inc;

  Vector2 dir;
  dir.set_as_angle(m_heading);

  TankMetric tm(this, pt, obs);

  // cout << "target:"
  //   " d=" << tm.target_distance() <<
  //   " a=" << tm.target_angle() <<
  //   " s=" << tm.target_side() <<
  //   endl;
  
  for(;;) {

    if( sidestep_obstacle(tm, h_inc, p_inc) ) break;

    if( turn_away_from_obstacle(dir, tm, h_inc, p_inc) ) break;

    if( detect_obstacle( tm )) break;
	
    if( turn_toward_target(tm, h_inc, p_inc) ) break;

    if( move_toward_target(tm, h_inc, p_inc) ) break;

    if( shoot_at_target(tm, h_inc, p_inc ) ) break;

    return; // no movement
  }

  m_heading  += h_inc;
  m_position += p_inc; 

  m_direction.set_as_angle( m_heading );

  m_mesh_instance.set_translation( m_position.to_vector3(m_height) );
  m_mesh_instance.set_rotation( M_PI - m_heading, 0, 0 );

  m_mesh_instance.transform();
}

bool MidTank::shoot_at_target( TankMetric& tm, float &h_inc, Vector2 &p_inc ) {
  
  m_fire_control.scan(tm);

  if( !m_fire_control.is_locked_on() ) return false;

  if( m_fire_control.should_fire() ) fire();

  return true;
}

bool MidTank::sidestep_obstacle( TankMetric& tm, float &h_inc, Vector2 &p_inc ) {

  if( m_obstacle.mode() != TankObstacle::OM_MOVE ) return false;

  if( m_obstacle.distance_to( m_position ) < 8 ) {
    p_inc = m_direction * 0.2;
    return true;
  }

  m_obstacle.clear();

  return false;
}

bool MidTank::turn_away_from_obstacle( Vector2& dir, TankMetric& tm, float &h_inc, Vector2 &p_inc ) {

  if( m_obstacle.mode() != TankObstacle::OM_TURN ) return false;

  cout << "turn_away_from_obstacle: angle_to="
       << m_obstacle.angle_to(dir)
       << endl;
  
  if( m_obstacle.angle_to( dir ) > 0.5 ) {
    if( m_obstacle.side() < 0 )
      h_inc = 0.05;
    else
      h_inc = -0.05;
    
  } else
    m_obstacle.next_mode();

  return true;
}

bool MidTank::detect_obstacle( TankMetric &tm ) {

  if( m_obstacle.mode() != TankObstacle::OM_INACTIVE ) return false;
  
  if( tm.obstacle_distance() > 5 ) return false;

  if( tm.obstacle_angle() < 0.9 ) return false;

  m_obstacle.set(m_position, tm);

  return true;
}

bool MidTank::turn_toward_target( TankMetric& tm, float &h_inc, Vector2 &p_inc ) {

  float tolerance = 0.7;

  for( ;; ) {

    if( tm.target_distance() < 7 ) {
      tolerance = 0.995;
      break;
    }

    if( tm.target_distance() < 15 ) {
      tolerance = 0.98;
      break;
    }

    if( tm.target_distance() < 30 ) {
      tolerance = 0.95;
      break;
    }

    if( tm.target_distance() < 60 ) {
      tolerance = 0.88;
    }

    break;
  }

  if( tm.target_angle() > tolerance ) return false;

  if( tm.target_side() > 0 )
    h_inc = 0.01;
  else
    h_inc = -0.01;

  return true; 
}

bool MidTank::move_toward_target( TankMetric& tm, float &h_inc, Vector2 &p_inc ) {
  
  if( tm.target_distance() < 5 ) return false;

  p_inc = m_direction * 0.2;

  return true;
}
