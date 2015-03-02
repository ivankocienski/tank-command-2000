
#pragma once

#include <vector>

#include "mesh.hh"
#include "bounding-box-2.hh"
#include "math/vector3.hh"
#include "math/vector2.hh"

class MeshInstance {
private:

  Mesh *m_mesh;

  std::vector<Vector3> m_vertices;
  Vector3 m_translate;
  Vector3 m_rotate;

  BoundingBox2D m_bounds;
    
  int m_color;

public:

  MeshInstance();
  MeshInstance( Mesh* );

  Vector3 & position();

  void set_translation( float, float, float );
  void set_translation( const Vector3& );
  void set_rotation( float, float, float );
  void set_color( int );
  int color();

  const BoundingBox2D & bounds() const;
  
  void transform();

  std::vector<Vector3> & vertices();
  const Mesh * mesh();

  // -1 hidden
  //  0 partial
  // +1 visible
  int classify_side( const Plane& );
};
