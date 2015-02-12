
#include <cfloat>
#include <cstdio>

#include <iostream>

#include "mesh.hh"

using namespace std;

Mesh::Mesh( int c ) {
  m_color = c;
}

Mesh::Mesh() {
  m_color = 255;  
}

bool Mesh::load_from( const char *filepath ) {

  bool okay = false;
  FILE *file;
  
  char sig[5] = { 0, 0, 0, 0, 0 };
  int vert_count;
  int edge_count;

  float   *vert_read_buffer = NULL;
  int32_t *edge_read_buffer = NULL;

  for(;;) {

    file = fopen( filepath, "r" );
    if(!file) break;

    fread( sig, sizeof(char), 4, file );
    if( strcmp( sig, "VEC1" )) break; 

    fread( &vert_count, sizeof(int32_t), 1, file );
    fread( &edge_count, sizeof(int32_t), 1, file );

    vert_read_buffer = (float*)calloc( vert_count * 3, sizeof(float));
    if(!vert_read_buffer) break;

    edge_read_buffer = (int32_t*)calloc( edge_count * 2, sizeof(int32_t));
    if(!edge_read_buffer) break;

    fread( vert_read_buffer, vert_count * 3, sizeof(float), file );
    fread( edge_read_buffer, edge_count * 2, sizeof(int32_t), file );

    float *vp = vert_read_buffer;
    for( int i = 0; i < vert_count; i++ ) {
      m_vertices.push_back( Vector3( vp[0], vp[1], vp[2] ));
      vp += 3;
    }

    int32_t *ep = edge_read_buffer;
    for( int i = 0; i < edge_count; i++ ) {
      m_edges.push_back( T_EDGE( ep[0], ep[1] ));
      ep += 2;
    }

    okay = true;

    break;
  }

  cout << "mesh load " << (okay ? "okay" : "fail") << endl;
  cout << "  vertex.size=" << m_vertices.size() << endl;
  cout << "  edge.count=" << m_edges.size() << endl;

  if(file) fclose(file);
  if(vert_read_buffer) free( vert_read_buffer );
  if(edge_read_buffer) free( edge_read_buffer );

  return okay;
}

const vector<Vector3> Mesh::vertices() const {
  return m_vertices;
}

const vector<Mesh::T_EDGE> & Mesh::edges() const {
  return m_edges;
}

int Mesh::color() {
  return m_color;
}

