

#include <iostream>
using std::cout;
using std::endl;

#include <math.h>

#include "common.hh"
#include "camera.hh"

static const float DEG_2_RAD = 0.017453292;

Camera::Camera(Window& w) : m_window(w) {
  m_near = 0;
  m_far  = 0;
  m_fovy = 0;
  m_aspect_ratio = 0;
}

void Camera::setup( float fovx, int xres, int yres, float near, float far) {

  m_xres = xres;
  m_yres = yres;

  m_aspect_ratio = (float)xres / (float)yres;
  m_near = near;
  m_far  = far;
  m_fovy = fovx * (1.0 / m_aspect_ratio);
}

void Camera::look( const Vector3& c_pos, const Vector3& c_dir, const Vector3& c_right ) {

  m_position = c_pos;
  m_forward  = c_dir;
  m_up       = Vector3( 0, 1, 0 );
  m_right    = c_right;

  Vector3 pos;
  Vector3 norm;

  // now set up the frustrum planes
  // near clip field
  pos  = m_position + (m_forward * m_near);
  norm = m_forward;
  m_clip_plane[CP_NEAR].setup( pos, norm );

  // far clip field
  pos  = m_position + (m_forward * m_far);
  norm = -m_forward;
  m_clip_plane[CP_FAR].setup( pos, norm );

  // some values

  float tang   = tan( DEG_2_RAD * m_fovy * 0.5 );
  float near_h = m_near * tang;
  float near_w = near_h * m_aspect_ratio;

  float far_h = m_far * tang * 0.5;
  float far_w = far_h * m_aspect_ratio;

  Vector3 near_tl( m_clip_plane[CP_NEAR].position() - (m_right * near_w) - (m_up * near_h) );
  Vector3 near_tr( m_clip_plane[CP_NEAR].position() + (m_right * near_w) - (m_up * near_h) );
  Vector3 near_bl( m_clip_plane[CP_NEAR].position() - (m_right * near_w) + (m_up * near_h) );
  Vector3 near_br( m_clip_plane[CP_NEAR].position() + (m_right * near_w) + (m_up * near_h) );

  Vector3 far_tl( m_clip_plane[CP_FAR].position() - (m_right * far_w) - (m_up * far_h) );
  Vector3 far_tr( m_clip_plane[CP_FAR].position() + (m_right * far_w) - (m_up * far_h) );
  Vector3 far_bl( m_clip_plane[CP_FAR].position() - (m_right * far_w) + (m_up * far_h) );
  Vector3 far_br( m_clip_plane[CP_FAR].position() + (m_right * far_w) + (m_up * far_h) );

//  // near
//  m_window.draw_line(
//      320 + 50 * near_tl.x,
//      240 + 50 * near_tl.z,
//      320 + 50 * near_tr.x,
//      240 + 50 * near_tr.z,
//      255);
//
//  // far
//  m_window.draw_line(
//      320 + 50 * far_tl.x,
//      240 + 50 * far_tl.z,
//      320 + 50 * far_tr.x,
//      240 + 50 * far_tr.z,
//      255);

  Vector3 e1;
  Vector3 e2;

  // left hand side (counter clockwise)
  e1   = near_tl - near_bl;
  e2   = near_bl - far_bl;
  norm = e2.cross(e1);
  norm.normalize();

  m_clip_plane[CP_LEFT].setup( (near_tl + near_bl + far_tl + far_bl) * 0.25, norm );

  // right
  e1   = far_tr - far_br;
  e2   = far_br - near_br;
  norm = e2.cross(e1);
  norm.normalize();

  m_clip_plane[CP_RIGHT].setup( (near_tr + near_br + far_tr + far_br) * 0.25, norm );







  { 
    // top
    e1   = near_tl - near_tr;
    e2   = near_tr - far_tr;
    norm = e1.cross(e2);
    norm.normalize();

    Vector3 pos = (near_tl + near_tr + far_tl + far_tr) * 0.25;
    m_clip_plane[CP_TOP].setup( pos, norm );

//    // top
//    m_window.draw_line(
//        320 + 50 * near_tl.x,
//        240 + 50 * near_tl.y,
//        320 + 50 * far_tl.x,
//        240 + 50 * far_tl.y,
//        255);
//
//    m_window.draw_line(
//        320 + 50 * pos.x,
//        240 + 50 * pos.y,
//        320 + 50 * pos.x + (20 * norm.x),
//        240 + 50 * pos.y + (20 * norm.y),
//        1
//    );
  }

  {
    // bottom
    e1   = near_br - near_bl;
    e2   = near_bl - far_bl;
    norm = e1.cross(e2);
    norm.normalize();

    Vector3 pos = (near_bl + near_br + far_bl + far_br) * 0.25;

    m_clip_plane[CP_BOTTOM].setup( pos, norm );

//    // bottom
//    m_window.draw_line(
//        320 + 50 * near_br.x,
//        240 + 50 * near_br.y,
//        320 + 50 * far_br.x,
//        240 + 50 * far_br.y,
//        255);
//
//    m_window.draw_line(
//        320 + 50 * pos.x,
//        240 + 50 * pos.y,
//        320 + 50 * pos.x + (20 * norm.x),
//        240 + 50 * pos.y + (20 * norm.y),
//        1
//    );
  }

  
}

Matrix4 Camera::translation_matrix() {

  Matrix4 trn_mat;
  trn_mat.identity();

  trn_mat.m_v[3]  = -m_position.x;
  trn_mat.m_v[7]  = -m_position.y;
  trn_mat.m_v[11] = -m_position.z;

  // just a heading rotation for now

  Matrix4 rot_mat;
  rot_mat.identity();

  // this needed to have the x and z components swapped
  // for some reason. sigh.
  float s = m_forward.x; // sin(d)
  float c = m_forward.z; // cos(d)

  rot_mat.m_v[0]  =  c;
  rot_mat.m_v[2]  = -s;
  rot_mat.m_v[8]  =  s;
  rot_mat.m_v[10] =  c;

  return rot_mat * trn_mat; // do translation first. A then B is BxA
}

const Plane *Camera::clip_planes() {
  return m_clip_plane;
}

void Camera::draw_3d_line( const Vector3& p1, const Vector3& p2, int c ) {

#if 0
  int p1_x = 320 + 50 * p1.x;
  int p1_y = 240 + 50 * p1.y;
  int p2_x = 320 + 50 * p2.x;
  int p2_y = 240 + 50 * p2.y;

  m_window.draw_line( p1_x, p1_y, p2_x, p2_y, c ); 
#endif



    // oh the magiks...
  float zoom_y =    1.0 / (tan(m_fovy / 2.0));
  float zoom_x = zoom_y / m_aspect_ratio;

  // precompute clip matrix elements
  float c10 = (m_far + m_near) / (m_far - m_near);
  float c14 = (2 * m_near * m_far ) / (m_near - m_far);

  int p1_x, p1_y;
  int p2_x, p2_y;
  
  // p1
  {
    // clip_matrix * p1
    float p1_a = p1.x * zoom_x;
    float p1_b = p1.y * zoom_y;
    float p1_c = p1.z * c10 + 1;
    float p1_d = p1.z * c14;

    if( p1_c < p1_d ) return;

    // project p1 to screen ...
    p1_x =   (p1_a * m_xres) / (2 * p1_d)  + (m_xres / 2);
    p1_y = -((p1_b * m_yres) / (2 * p1_d)) + (m_yres / 2); 
  }

  // p2
  {
    // clip_matrix * p1
    float p2_a = p2.x * zoom_x;
    float p2_b = p2.y * zoom_y;
    float p2_c = p2.z * c10 + 1;
    float p2_d = p2.z * c14;

    if( p2_c < p2_d ) return;

    // project p1 to screen ...
    p2_x =   (p2_a * m_xres) / (2 * p2_d)  + (m_xres / 2);
    p2_y = -((p2_b * m_yres) / (2 * p2_d)) + (m_yres / 2); 
  }

  m_window.draw_line( p1_x, p1_y, p2_x, p2_y, c ); 
}

const Vector3& Camera::direction() {
  return m_forward;
}

