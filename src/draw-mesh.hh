
#pragma once

#include "math/matrix4.hh"
#include <vector>
#include <list>

#include "mesh-instance.hh"

class Camera;

class DrawLine {
private:

  Vector3 m_p1;
  Vector3 m_p2;

  bool m_active;

  void project_and_clip( const Plane&, Vector3&, Vector3& );
public:

  DrawLine(const Vector3&, const Vector3&);
  void dump();
  
  void clip_to( const Plane& );
  void transform( Matrix4& );
  bool active();
  void draw(Camera*, int);

  const Vector3 & p1() const;
  const Vector3 & p2() const;
};

class DrawMesh {
private:

  MeshInstance *m_mesh_instance;
  Camera       *m_camera;

  std::list<DrawLine> m_draw_lines;
  
  enum { // visibility
    V_NONE,
    V_PARTIAL,
    V_FULL
  };

  int m_visibility;
  float m_cam_dist;

public:

  DrawMesh();
  DrawMesh( MeshInstance*, Camera* );

  void clip_to_frustum();

  bool is_visible();

  void camera_transform( );

  void draw( );

  float camera_distance() const;
};

