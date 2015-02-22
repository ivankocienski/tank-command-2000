
#include "mid-tank.hh"
#include "../assets.hh"

using namespace std;

static const float NEAR_MAX = 1000000;

/* TankMetric
 *
 * Compute and store various metrics for the tanks behaviour modules
 *
 */

TankMetric::TankMetric( Vector3 &me_pos, Vector3 &me_dir, PlayerTank *tgt, std::vector<MeshInstance>& obs ) {

  m_obs_distance = NEAR_MAX;
  m_obs_angle    = 0;
  m_obs_side     = 0;

  m_tgt_distance = NEAR_MAX;
  m_tgt_angle    = 0;
  m_tgt_side     = 0;

  Vector3 me_dir_perp = me_dir.perpendicular();

  // target

  Vector3 tgt_vector = tgt->position() - me_pos;

  m_tgt_distance = tgt_vector.magnitude();
  tgt_vector.normalize();
  m_tgt_angle    = me_dir.dot(tgt_vector); 
  m_tgt_side     = tgt_vector.dot(me_dir_perp);

  // obstacles

  float near = NEAR_MAX;
  Vector3 near_vto;

  vector<MeshInstance>::iterator it;
  for( it = obs.begin(); it != obs.end(); it++ ) {

    Vector3 vector = it->position() - me_pos;
    float dist = vector.magnitude();

    if( dist >= near ) continue;

    near = dist;
    near_vto.set( vector );
  }

  if( near < NEAR_MAX) {
    m_obs_angle    = me_dir.dot(m_obs_vector);
    m_obs_side     = near_vto.dot(me_dir_perp);
    near_vto.normalize();
    m_obs_vector   = near_vto;
    m_obs_distance = near;
  }
}

float TankMetric::obstacle_distance()   { return m_obs_distance; } 
float TankMetric::obstacle_angle()      { return m_obs_angle; } 
float TankMetric::obstacle_side()       { return m_obs_side; } 
Vector3 & TankMetric::obstacle_vector() { return m_obs_vector; }

float TankMetric::target_angle()      { return m_tgt_angle; } 
float TankMetric::target_distance()   { return m_tgt_distance; } 
float TankMetric::target_side()       { return m_tgt_side; } 

/* TankFireControl
 *
 * Wraps the control of the tanks gun
 */

TankFireControl::T_FIRE_TABLE TankFireControl::s_fire_table [] = {
  /* distance, hold */
  {  0,  20, 50 },
  {  1,  50, 30 },
  {  2, 100, 20 },
  {  3, 200, 10 },
  { -1,   0,  0 }
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
      m_should_fire  = (m_fire_hold % 10);
      m_fire_hold--;
      return;
    }
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
  m_active = false;
  m_side   = 0;
}

bool TankObstacle::is_active() {
  return m_active;
}

void TankObstacle::set( Vector3 &my_pos, TankMetric &tm ) {
  m_active     = true;
  m_mark_point = my_pos;
  m_obs_vector = tm.obstacle_vector();
  m_side       = tm.obstacle_side();
}

float TankObstacle::distance_to( Vector3& p ) {
  return (m_mark_point - p).magnitude();
}

float TankObstacle::angle_to( Vector3& d ) {
  return m_obs_vector.dot(d);
}

float TankObstacle::side() {
  return m_side;
}

/* MidTank
 * 
 * the main tank class
 */

MidTank::MidTank() : m_mesh_instance( &g_mesh_list[ A_MID_TANK ]) {
  m_heading = 0;
  m_active = true;
}

void MidTank::fire() {
}

void MidTank::set_pos( float x, float y ) {
  m_position.set( x, 0, y );

  m_mesh_instance.set_translation( m_position );

  m_mesh_instance.transform();
}

bool MidTank::is_active() {
  return m_active;
}

MeshInstance & MidTank::mesh_instance() {
  return m_mesh_instance;
}

void MidTank::think_and_move( PlayerTank *pt, vector<MeshInstance> &obs ) {

  if( !m_active ) return;

  float h_inc = 0;
  Vector3 p_inc;

  Vector3 dir;
  dir.set_as_angle(m_heading);

  TankMetric tm(m_position, dir, pt, obs);
  
  for(;;) {
    if( shoot_at_target(tm, h_inc, p_inc ) ) break;

    if( sidestep_obstacle(tm, h_inc, p_inc) ) break;

    if( turn_away_from_obstacle(dir, tm, h_inc, p_inc) ) break;

    if( turn_toward_target(tm, h_inc, p_inc) ) break;

    if( move_toward_target(tm, h_inc, p_inc) ) break;

    return; // no movement
  }

  m_heading  += h_inc;
  m_position += p_inc;

  m_mesh_instance.set_translation( m_position );
  m_mesh_instance.set_rotation( m_heading, 0, 0 );

  m_mesh_instance.transform();
}

bool MidTank::shoot_at_target( TankMetric& tm, float &h_inc, Vector3 &p_inc ) {
  
  m_fire_control.scan(tm);

  if( !m_fire_control.is_locked_on() ) return false;

  if( m_fire_control.should_fire() ) fire();

  return true;
}

bool MidTank::sidestep_obstacle( TankMetric& tm, float &h_inc, Vector3 &p_inc ) {

  if( !m_obstacle.is_active() ) return false;

  if( m_obstacle.distance_to( m_position ) < 25 ) {
    p_inc = m_direction;
    return true;
  }

  m_obstacle.clear();

  return false;
}

bool MidTank::turn_away_from_obstacle( Vector3& dir, TankMetric& tm, float &h_inc, Vector3 &p_inc ) {

  if( !m_obstacle.is_active() ) {

    if( tm.obstacle_distance() > 50 ) return false;

    if( tm.obstacle_angle() < 0.5 ) return false;

    m_obstacle.set(m_position, tm);
  }

  if( m_obstacle.angle_to( dir ) > 0.01 ) {
    if( m_obstacle.side() > 0 )
      h_inc = 0.05;
    else
      h_inc = -0.05;
  }

  return true;
}

bool MidTank::turn_toward_target( TankMetric& tm, float &h_inc, Vector3 &p_inc ) {

  if( tm.target_angle() > 0.99 ) return false;

  if( tm.target_side() < 0 )
    h_inc = 0.05;
  else
    h_inc = -0.05;

  return true; 
}

bool MidTank::move_toward_target( TankMetric& tm, float &h_inc, Vector3 &p_inc ) {
  
  if( tm.target_distance() < 20 ) {
    if( tm.target_angle() < 0.995 ) return false;
  }

  if( tm.target_distance() < 50 ) {
    if( tm.target_angle() < 0.98 ) return false;
  }

  if( tm.target_distance() < 100 ) {
    if( tm.target_angle() < 0.95 ) return false;
  }

  if( tm.target_distance() < 170 ) {
    if( tm.target_angle() < 0.88 ) return false;
  }

  if( tm.target_angle() < 0.7 ) return false;

  p_inc = m_direction;

  return true;
}
