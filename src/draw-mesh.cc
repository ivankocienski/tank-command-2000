
#include <iostream>

#include "common.hh"
#include "draw-mesh.hh"

using namespace std;

/* 
 * draw line - deal with individual lines of a mesh
 * 
 */

DrawLine::DrawLine( const Vector3 &p1, const Vector3 &p2 ) : m_p1(p1), m_p2(p2) {
  m_active     = true;
  m_face_count = 0;
}

bool DrawLine::active() {
  return m_active;
}

void DrawLine::transform( Matrix4& mat ) {

  if( !m_active ) return;

  // *cough* multiply a 3x1 vector by a 4x4 matrix *cough*

  {
    float nx = m_p1.x * mat[0] + m_p1.y * mat[1] + m_p1.z * mat[2]  + mat[3];
    float ny = m_p1.x * mat[4] + m_p1.y * mat[5] + m_p1.z * mat[6]  + mat[7];
    float nz = m_p1.x * mat[8] + m_p1.y * mat[9] + m_p1.z * mat[10] + mat[11];

    m_p1.set( nx, ny, nz );
  } 

  {
    float nx = m_p2.x * mat[0] + m_p2.y * mat[1] + m_p2.z * mat[2]  + mat[3];
    float ny = m_p2.x * mat[4] + m_p2.y * mat[5] + m_p2.z * mat[6]  + mat[7];
    float nz = m_p2.x * mat[8] + m_p2.y * mat[9] + m_p2.z * mat[10] + mat[11];

    m_p2.set( nx, ny, nz );
  } 
}

void DrawLine::project_and_clip( const Plane &pln, Vector3& p1, Vector3& p2 ) {

  // p1 is always 'in front' of the plane

  //if( g_capture )
  //  cout << "project_and_clip" << endl;

  Vector3 dir = (p1 - p2);
  float   len = dir.magnitude();

  //if( g_capture ) {
  //  dir.dump();
  //  cout << "len=" << len << endl;
 // }

  dir.normalize();

  float b = dir.dot( pln.normal() );
  if( b == 0 ) return; // is parallel

  float a = (p1 - pln.position()).dot(pln.normal());

  //if( g_capture )
  //  cout << "a=" << a << "  b=" << b << endl;

  float d = a / b;
  //if( g_capture ) 
  //  cout << "len=" << len << "  d=" << d << endl;

  if( d > len ) return; // is beyond us.

  p2.set( p1 - (dir * d) );
}

void DrawLine::clip_to( const Plane& pln ) {

  // so one of our points may behind the plane

  bool a = pln.is_facing( m_p1 );
  bool b = pln.is_facing( m_p2 );

  if( a && b ) {
    if( g_capture )
      cout << "  is facing" << endl;
    return;
  }

  if( !(a || b )) {
    if( g_capture )
      cout << "  is hidden" << endl;
    m_active = false;
    return;
  }

  if( g_capture ) 
    cout << "  is partial" << endl;

  if(a) { 
    project_and_clip( pln, m_p1, m_p2 );

  } else {
    project_and_clip( pln, m_p2, m_p1 );
  }
  
  // else b

}

void DrawLine::draw( Camera &cam, int c ) {
  if( !m_active ) return;
  cam.draw_3d_line( m_p1, m_p2, c );
}

void DrawLine::add_face() {
  m_face_count++;
}

void DrawLine::remove_face() {
  m_face_count--;
  if(!m_face_count) m_active = false;
}

void DrawLine::dump() {
  if( g_capture )
    cout << "  DrawLine: m_face_count=" << m_face_count << "  m_active=" << m_active << endl;
}

/* 
 * the draw mesh code
 *
 */

// not the fastest way of doing this ...

DrawMesh::DrawMesh( MeshInstance &mi, Camera &c ) : m_mesh_instance(mi), m_camera(c) {
  m_visibility = V_NONE;

  const vector<Vector3>& v = mi.vertices();

  const Mesh *m = m_mesh_instance.mesh();

  vector<Mesh::T_EDGE>::const_iterator e_it; 
  for( e_it = m->edges().begin(); e_it != m->edges().end(); e_it++ ) {

    m_draw_lines.push_back( 
      DrawLine( v[ e_it->p1 ], v[ e_it->p2 ] )
    );

  }
}

void DrawMesh::clip_to_frustum() {

  const Plane *clip_plane = m_camera.clip_planes();
  int partial_plane[6];
  int partial_plane_pos = 0;

  //for( int i = 0; i < 6; i++ ) {
  for( int i = 0; i < 6; i++ ) {
    
    int s = m_mesh_instance.classify_side(clip_plane[i]);

    // behind plane
    if( s < 0 ) return;

    // in front of plane 
    if( s > 0 ) continue;

    // partially in front of plane
    partial_plane[partial_plane_pos++] = i; 
  }

  // so if we have no partial planes, we must be completely visible. YAY
  if( partial_plane_pos == 0 ) {
    m_visibility = V_FULL;
    return;
  }

  if( g_capture )
    cout << "  partial clip" << endl;

  for( int i = 0; i < partial_plane_pos; i++ ) {

    for( list<DrawLine>::iterator it = m_draw_lines.begin(); it != m_draw_lines.end(); ) {
      it->clip_to( clip_plane[ partial_plane[i] ] );

      if( it->active() ) {
        it++;
        continue;
      }

      it = m_draw_lines.erase(it);
    } 
  } 

  m_visibility = V_PARTIAL;
}

bool DrawMesh::is_visible() {
  return m_visibility != V_NONE;
}

void DrawMesh::camera_transform( ) {
  
  Matrix4 cam_mat = m_camera.translation_matrix();

  for( list<DrawLine>::iterator it = m_draw_lines.begin(); it != m_draw_lines.end(); it++ ) {
    it->transform( cam_mat );
  }
}

void DrawMesh::draw() {

  int c = 255; //m_mesh.color();

  for( list<DrawLine>::iterator it = m_draw_lines.begin(); it != m_draw_lines.end(); it++ ) {
    it->draw(m_camera, c);
  }
}

