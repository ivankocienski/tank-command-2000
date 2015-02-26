
#include <iostream>
using std::cout;
using std::endl;

#include <math.h>

#include "bullet.hh"
#include "assets.hh"

static const float c_bullet_speed = 1;
static const float c_bullet_height = 1.3;

using namespace std;

Bullet::Bullet() {
  m_angle = 0;
  m_alive = 0;
}

Bullet::Bullet( Vector2& pos, float an, int al ) : m_mesh_instance( &g_mesh_list[ A_BULLET ]) {
  m_position = pos;
  m_angle    = an;
  m_alive    = al;

  m_direction.set_as_angle(m_angle); 

  m_mesh_instance.set_rotation( M_PI - m_angle, 0, 0 );
  m_mesh_instance.set_translation( m_position.x, c_bullet_height, m_position.y );

  m_mesh_instance.transform();
}

void Bullet::move() {

  if( m_alive ) {
    m_position += m_direction * c_bullet_speed;
    m_alive--;

    m_mesh_instance.set_translation( m_position.x, c_bullet_height, m_position.y );

    m_mesh_instance.transform();
  }
}

MidTank* Bullet::has_hit_enemy( vector<MidTank> & el ) {

  for( vector<MidTank>::iterator it = el.begin(); it != el.end(); it++ ) {

    MeshInstance &mi = it->mesh_instance();
    
    if( mi.point_inside_bb( m_position )) return &(*it);
  }

  return NULL;
}

bool Bullet::has_hit_obstacle( vector<Obstacle> & wo ) {

  for( vector<Obstacle>::iterator it = wo.begin(); it != wo.end(); it++ ) {

    MeshInstance &mi = it->mesh();
    
    if( mi.point_inside_bb( m_position )) return true;
  }

  return false;
}

bool Bullet::is_active() {
  return m_alive > 0;
  
}

MeshInstance & Bullet::mesh() {
  return m_mesh_instance; 
}

