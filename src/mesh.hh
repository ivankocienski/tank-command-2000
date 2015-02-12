
#pragma once

#include "math/vector3.hh"
#include <vector>

#include "plane.hh"
#include "window.hh"

class Mesh {
public:

  typedef struct _S_EDGE {
    int p1;
    int p2;
    _S_EDGE( int _p1, int _p2 ) { p1 = _p1; p2 = _p2; }
  } T_EDGE, *PT_EDGE;

private:

  std::vector<Vector3> m_vertices;

  std::vector<T_EDGE> m_edges;

  int m_color;


public:

  Mesh(int);
  Mesh();

  bool load_from( const char* );

  const std::vector<Vector3> vertices() const;
  const std::vector<T_EDGE> & edges() const;

  int color();
};
