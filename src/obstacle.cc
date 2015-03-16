
#include <iostream>
using std::cout;
using std::endl;

#include "obstacle.hh"

#include "assets.hh"

Obstacle::Obstacle() {
  m_height = 0;
  m_angle  = 0;
}

Obstacle::Obstacle( int m ) : m_mesh( &g_mesh_list[m] ) {
  m_height = 0;
  m_angle  = 0;
}

void Obstacle::set_pos( float x, float y, float z, float a ) {
  m_position.set( x, y );
  m_height = z;
  m_angle  = a;

  m_mesh.set_color( 2 + rand() % 7 );
  m_mesh.set_translation( m_position.to_vector3(m_height) );
  m_mesh.set_rotation( 0, a, 0 );

  m_mesh.transform();

  cout << "Obstacle::set_pos radius=" << m_mesh.bounds().radius() << endl;
}

MeshInstance & Obstacle::mesh() {
  return m_mesh;
}

Vector2 & Obstacle::position() {
  return m_position;
}
