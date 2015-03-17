
#include "powerup.hh"
#include "assets.hh"

Powerup::Powerup() {
  m_alive =  0;
  m_type  = -1;
}

Powerup::Powerup(const Vector2 &p, int t) {
  m_position = p;
  m_type     = t;
  m_angle    = 0;
  m_alive    = 200;

  Mesh *m;
  switch( t ) {
    case PT_SCORE:  m = &g_mesh_list[ A_POWERUP_SCORE ]; break;
    case PT_ARMOUR: m = &g_mesh_list[ A_POWERUP_ARMOUR ]; break;
  }

  m_mesh_instance.set_mesh(m);
  m_mesh_instance.set_translation( p.to_vector3( 0.4 ));

  update();
}

void Powerup::update() {

  if( !m_alive ) return;

  m_alive--;
  m_angle += 0.1; 
  
  m_mesh_instance.set_rotation( m_angle, 0, 0 );

  m_mesh_instance.transform(); 
}

bool Powerup::is_active() {
  return m_alive > 0;
}

int Powerup::type() {
  return m_type;
}

MeshInstance & Powerup::mesh_instance() {
  return m_mesh_instance;
}

const Vector2 & Powerup::position() {
  return m_position;
}
