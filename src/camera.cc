

#include <iostream>
using std::cout;
using std::endl;

#include <math.h>

#include "common.hh"
#include "camera.hh"

static const float DEG_2_RAD = 0.017453292;

Camera::Camera(Window& w) : m_window(w) {
  m_xres = 640; //w.width();
  m_yres = 400; //w.height();
  m_aspect_ratio = 1.6; //w.aspect_ratio();

  m_near = 0;
  m_far  = 0;
  m_fovx = 0;
}

// to try: !!!!!!!!!!!!!!!!!!!!!!! <<<<<<<<-------------------
//   lets try storing fov as x (90 degrees) and doing the Y fov
//   conversion thingy

void Camera::setup( float fovx, float near, float far) {

  m_near = near;
  m_far  = far;
  m_fovx = fovx;

  // oh the magiks...
  m_zoom_x = 1.0 / tan(DEG_2_RAD * m_fovx * 0.7);
  m_zoom_y = m_zoom_x * m_aspect_ratio; 

  cout << "camera setup" << endl;
  cout << "  xres         =" << m_xres << endl;
  cout << "  yres         =" << m_yres << endl;
  cout << "  aspect_ratio =" << m_aspect_ratio << endl;
  cout << "  near         =" << m_near << endl;
  cout << "  far          =" << m_far << endl;
  cout << "  fovx         =" << m_fovx << endl;
  //cout << "  fovy         =" << m_fovy << endl;
  cout << "  zoom_x       =" << m_zoom_x << endl;
  cout << "  zoom_y       =" << m_zoom_y << endl;
  cout << endl;
  cout << "  zx/zy=" << (m_zoom_x / m_zoom_y) << endl;
  cout << "  zy/zx=" << (m_zoom_y / m_zoom_x) << endl;
}

void Camera::look( const Vector3& c_pos, const Vector3& c_dir, const Vector3& c_right ) {

  m_position = c_pos;
  m_forward  = c_dir;
  m_up       = Vector3( 0, 1, 0 );
  m_right    = c_right;

  m_pos2D.set( m_position.x, m_position.z );

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

  float tang   = tan( DEG_2_RAD * m_fovx * 0.5 );
  float near_w = m_near * tang;
  float near_h = near_w / m_aspect_ratio;

  float far_w = m_far * tang; 
  float far_h = far_w / m_aspect_ratio;

  Vector3 near_tl( m_clip_plane[CP_NEAR].position() - (m_right * near_w) - (m_up * near_h) );
  Vector3 near_tr( m_clip_plane[CP_NEAR].position() + (m_right * near_w) - (m_up * near_h) );
  Vector3 near_bl( m_clip_plane[CP_NEAR].position() - (m_right * near_w) + (m_up * near_h) );
  Vector3 near_br( m_clip_plane[CP_NEAR].position() + (m_right * near_w) + (m_up * near_h) );

  Vector3 far_tl( m_clip_plane[CP_FAR].position() - (m_right * far_w) - (m_up * far_h) );
  Vector3 far_tr( m_clip_plane[CP_FAR].position() + (m_right * far_w) - (m_up * far_h) );
  Vector3 far_bl( m_clip_plane[CP_FAR].position() - (m_right * far_w) + (m_up * far_h) );
  Vector3 far_br( m_clip_plane[CP_FAR].position() + (m_right * far_w) + (m_up * far_h) );

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
  }

  {
    // bottom
    e1   = near_br - near_bl;
    e2   = near_bl - far_bl;
    norm = e1.cross(e2);
    norm.normalize();

    Vector3 pos = (near_bl + near_br + far_bl + far_br) * 0.25;

    m_clip_plane[CP_BOTTOM].setup( pos, norm ); 
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

  // precompute clip matrix elements
  float c10 = (m_far + m_near) / (m_far - m_near);
  float c14 = (2 * m_near * m_far ) / (m_near - m_far);

  int p1_x, p1_y;
  int p2_x, p2_y;
  
  // p1
  {
    // clip_matrix * p1
    float p1_a = p1.x * m_zoom_x;
    float p1_b = p1.y * m_zoom_y;
    float p1_c = p1.z * c10 + 1;
    float p1_d = p1.z * c14;

    if( p1_c < p1_d ) return;

    // project p1 to screen ...
    p1_x = -(p1_a * m_xres) / (2 * p1_d)  + (m_xres / 2);
    p1_y = ((p1_b * m_yres) / (2 * p1_d)) + (m_yres / 2); 
  }

  // p2
  {
    // clip_matrix * p1
    float p2_a = p2.x * m_zoom_x;
    float p2_b = p2.y * m_zoom_y;
    float p2_c = p2.z * c10 + 1;
    float p2_d = p2.z * c14;

    if( p2_c < p2_d ) return;

    // project p1 to screen ...
    p2_x = -(p2_a * m_xres) / (2 * p2_d)  + (m_xres / 2);
    p2_y = ((p2_b * m_yres) / (2 * p2_d)) + (m_yres / 2); 
  }


  p1_y += 80;
  p2_y += 80;

  m_window.draw_line( p1_x, p1_y, p2_x, p2_y, c ); 
}

const Vector3& Camera::direction() const {
  return m_forward;
}

const Vector3& Camera::position() const {
  return m_position;
}

const Vector2& Camera::position2D() const {
  return m_pos2D;
}

