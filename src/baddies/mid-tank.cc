
#include "mid-tank.hh"
#include "../assets.hh"

MidTank::MidTank() : m_mesh_instance( &g_mesh_list[ A_MID_TANK ]) {
  m_heading = 0;
  m_active = true;
}

void MidTank::set_pos( float x, float y ) {
  m_position.set( x, 0, y );

  m_mesh_instance.set_translation( m_position );

  m_mesh_instance.transform();
}

void MidTank::think_and_move() {

  if( !m_active ) return;

  bool has_moved = false;

  //...

  if( has_moved )
    m_mesh_instance.transform();
}

bool MidTank::is_active() {
  return m_active;
}

MeshInstance & MidTank::mesh_instance() {
  return m_mesh_instance;
}
