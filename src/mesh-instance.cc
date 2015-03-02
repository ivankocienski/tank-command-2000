
#include <cmath>

using namespace std;

#include "mesh-instance.hh"
#include "math/operations.hh"

MeshInstance::MeshInstance() {
  m_mesh  = NULL;
  m_color = 255;
}

MeshInstance::MeshInstance( Mesh* m ) {
  m_mesh  = m;
  m_color = 255;
}

void MeshInstance::set_translation( float xo, float yo, float zo ) {
  m_translate.set( xo, yo, zo ); 
}

void MeshInstance::set_translation( const Vector3& p ) {
  m_translate.set( p ); 
}

Vector3 & MeshInstance::position() {
  return m_translate;
}

void MeshInstance::set_color( int c ) {
  m_color = c;
}

int MeshInstance::color() {
  return m_color;
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

  m_bounds.reset();
  
  // re-calculate AABB
  for( it = m_vertices.begin(); it != m_vertices.end(); it++ ) {

    m_bounds.update( *it );
    
  }

  m_bounds.finish();
}

vector<Vector3> & MeshInstance::vertices() {
  return m_vertices;
}

const Mesh * MeshInstance::mesh() {
  return m_mesh;
}



const BoundingBox2D & MeshInstance::bounds() const {
  return m_bounds;
}
