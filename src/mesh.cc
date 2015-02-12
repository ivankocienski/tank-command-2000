
#include <cfloat>

#include <iostream>
using std::cout;
using std::endl;
#include "mesh.hh"

using namespace std;

#define VECTOR(x, y) m_vertices.push_back( Vector3( x, 0, y ))
#define EDGE(a, b) m_edges.push_back( T_EDGE( a, b ) )

Mesh::Mesh( float xo, float yo, int c ) {
  
  m_color = c;

  Vector3 o (xo, 0, yo );

  float s = 0.45;

  // front
  m_vertices.push_back( o + Vector3( -s,  s, -s ));
  m_vertices.push_back( o + Vector3(  s,  s, -s ));
  m_vertices.push_back( o + Vector3(  s, -s, -s ));
  m_vertices.push_back( o + Vector3( -s, -s, -s ));

  // back
  m_vertices.push_back( o + Vector3( -s,  s,  s ));
  m_vertices.push_back( o + Vector3(  s,  s,  s ));
  m_vertices.push_back( o + Vector3(  s, -s,  s ));
  m_vertices.push_back( o + Vector3( -s, -s,  s ));

  // front
  m_edges.push_back( T_EDGE( 0, 1 ) );
  m_edges.push_back( T_EDGE( 1, 2 ) );
  m_edges.push_back( T_EDGE( 2, 3 ) );
  m_edges.push_back( T_EDGE( 3, 0 ) );
  
  // back
  m_edges.push_back( T_EDGE( 4, 5 ) );
  m_edges.push_back( T_EDGE( 5, 6 ) );
  m_edges.push_back( T_EDGE( 6, 7 ) );
  m_edges.push_back( T_EDGE( 7, 4 ) );

  calculate_limits();

  // front
  {
    m_faces.push_back(T_FACE());
    T_FACE &f = m_faces.back();

    f.edges.push_back( 0 );
    f.edges.push_back( 1 );
    f.edges.push_back( 2 );
    f.edges.push_back( 3 );

    f.normal.set( 0, 0, -1 );
  }

  // back 
  {
    m_faces.push_back(T_FACE());
    T_FACE &f = m_faces.back();

    f.edges.push_back( 4 );
    f.edges.push_back( 5 );
    f.edges.push_back( 6 );
    f.edges.push_back( 7 );

    f.normal.set( 0, 0, 1 );
  }

}

Mesh::Mesh() {

  Mesh( 0, 0, 255 );
  
  // arrow ...
//  VECTOR( 0,   0.5 );
//  VECTOR( 1,   0.5 );
//  VECTOR( 1,   1 );
//  VECTOR( 1.5, 0 );
//  VECTOR( 1,  -1 );
//  VECTOR( 1,  -0.5 );
//  VECTOR( 0,  -0.5 );
//
//  EDGE( 0, 1 );
//  EDGE( 1, 2 );
//  EDGE( 2, 3 );
//  EDGE( 3, 4 );
//  EDGE( 4, 5 );
//  EDGE( 5, 6 );
//  EDGE( 6, 0 );





  // TODO: top, bottom, left, right


  m_min.dump();
  m_max.dump();
}

void Mesh::calculate_limits() {

  //m_min.set( FLT_MAX, FLT_MAX, FLT_MAX );
  ///m_max.set( FLT_MIN, FLT_MIN, FLT_MIN );

  m_min.set( 1000, 1000, 1000 );
  m_max.set( -1000, -1000, -1000 );

  for( vector<Vector3>::iterator it = m_vertices.begin(); it != m_vertices.end(); it++ ) {

    cout << "vertex" << endl;

    if( it-> x < m_min.x ) m_min.x = it->x;
    if( it-> y < m_min.y ) m_min.y = it->y;
    if( it-> z < m_min.z ) m_min.z = it->z;

    if( it-> x > m_max.x ) m_max.x = it->x;
    if( it-> y > m_max.y ) m_max.y = it->y;
    if( it-> z > m_max.z ) m_max.z = it->z;

  }
}

const vector<Vector3> Mesh::vertices() const {
  return m_vertices;
}

const vector<Mesh::T_EDGE> & Mesh::edges() const {
  return m_edges;
}

const vector<Mesh::T_FACE> & Mesh::faces() const {
  return m_faces;
}

int Mesh::classify_side( const Plane &pln ) {

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


int Mesh::color() {
  return m_color;
}

