
#pragma once

#include "math/matrix4.hh"
#include "math/vector3.hh"
#include "plane.hh"
#include "mesh.hh"
#include "window.hh"

/* has two purposes -
 *   create a camera matrix to translate geometry to camera space
 *   reject geometry that is outside clip space */

class Camera {
private:

  enum {
    CP_NEAR,
    CP_FAR,
    CP_LEFT,
    CP_RIGHT,
    CP_TOP,
    CP_BOTTOM,
    CP_COUNT
  };

  Window &m_window;
  
  Plane m_clip_plane[CP_COUNT];

  Vector3 m_position;
  Vector3 m_forward;
  Vector3 m_right;
  Vector3 m_up;

  int m_xres;
  int m_yres;

  float m_near;
  float m_far;
  float m_fovy;
  float m_aspect_ratio;

public:

  Camera(Window&);

  void setup( float, float, float);

  void look( const Vector3&, const Vector3&, const Vector3& );

  Matrix4 translation_matrix();

  const Plane* clip_planes();
  const Vector3& direction();

  void draw_3d_line( const Vector3&, const Vector3&, int c );
};

