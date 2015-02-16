
#include <cmath>

using namespace std;

#include "mesh-instance.hh"
#include "math/operations.hh"

MeshInstance::MeshInstance() {
  m_mesh = NULL;
}

MeshInstance::MeshInstance( Mesh* m ) {
  m_mesh = m;
}

void MeshInstance::set_translation( float xo, float yo, float zo ) {
  m_translate.set( xo, yo, zo ); 
}

bool MeshInstance::point_inside_bb( Vector3& tp ) {

  if( tp.x < m_min.x || tp.x > m_max.x ) return false;
  if( tp.y < m_min.y || tp.y > m_max.y ) return false;
  if( tp.z < m_min.z || tp.z > m_max.z ) return false; 

  return true; 
}

void MeshInstance::set_rotation( float r, float p, float y ) {
  // TODO: use of quaternions
  m_rotate.set( r, p, y );
}

void MeshInstance::transform() {

  vector<Vector3>::iterator it;

  m_vertices = m_mesh->vertices();

  // generate transform matrix
  Matrix4 translate;
  translate.set_translate( m_translate );

  Matrix4 rotate;
  rotate.set_rotation( m_rotate );

  Matrix4 transform = translate * rotate;

  // transform vertices

  for( it = m_vertices.begin(); it != m_vertices.end(); it++ )
    vec3_mat4_multiply( *it, transform );

  m_min.set(  10000,  10000,  10000 );
  m_max.set( -10000, -10000, -10000 );

  // re-calculate AABB
  for( it = m_vertices.begin(); it != m_vertices.end(); it++ ) {

    if( it-> x < m_min.x ) m_min.x = it->x;
    if( it-> y < m_min.y ) m_min.y = it->y;
    if( it-> z < m_min.z ) m_min.z = it->z;

    if( it-> x > m_max.x ) m_max.x = it->x;
    if( it-> y > m_max.y ) m_max.y = it->y;
    if( it-> z > m_max.z ) m_max.z = it->z;

  }
}

vector<Vector3> & MeshInstance::vertices() {
  return m_vertices;
}

const Mesh * MeshInstance::mesh() {
  return m_mesh;
}

int MeshInstance::classify_side( const Plane &pln ) {

  int i;

  Vector3 v;

  // left top back
  v.set( m_min.x, m_max.y, m_min.z );
  i += pln.is_facing(v);

  // right top back
  v.set( m_max.x, m_max.y, m_min.z );
  i += pln.is_facing(v);

  // left top front
  v.set( m_min.x, m_max.y, m_max.z );
  i += pln.is_facing(v);

  // right top front
  v.set( m_max.x, m_max.y, m_max.z );
  i += pln.is_facing(v);


  // left bottom back
  v.set( m_min.x, m_min.y, m_min.z );
  i += pln.is_facing(v);

  // right bottom back
  v.set( m_max.x, m_min.y, m_min.z );
  i += pln.is_facing(v);

  // left bottom front
  v.set( m_min.x, m_min.y, m_max.z );
  i += pln.is_facing(v);

  // right bottom front
  v.set( m_max.x, m_min.y, m_max.z );
  i += pln.is_facing(v);

  if( i == 8 ) return 1;
  if( i == 0 ) return -1;

  return 0;
}

