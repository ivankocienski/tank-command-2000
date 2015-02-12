
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

  typedef struct _S_FACE {
    std::vector<int> edges;
    Vector3 normal;
    _S_FACE() {}
  } T_FACE, *PT_FACE;

private:

  std::vector<Vector3> m_vertices;

  std::vector<T_EDGE> m_edges;
  std::vector<T_FACE> m_faces;

  int m_color;

  Vector3 m_min;
  Vector3 m_max;

  void calculate_limits();


public:

  Mesh(float, float, int);
  Mesh();

  const std::vector<Vector3> vertices() const;
  const std::vector<T_EDGE> & edges() const;
  const std::vector<T_FACE> & faces() const;

  // -1 hidden
  //  0 partial
  // +1 visible
  int classify_side( const Plane& );

  int color();
};
