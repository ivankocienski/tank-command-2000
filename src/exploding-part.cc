
#include "exploding-part.hh"
#include "assets.hh"
#include "math/operations.hh"

ExplodingPart::ExplodingPart() {
}

void ExplodingPart::init( const Vector2& p ) {

  m_count = 10 + frand() * 40;

  m_rot_alpha = 0;
  m_rot_beta  = 0;
  
  m_rot_alpha_inc = 0.5 * sfrand();
  m_rot_beta_inc  = 0.5 * sfrand();

  m_height   = 3 * 25 * frand();
  m_speed    = 0.3 + 0.7 * frand();
  m_position = p;

  m_theta = 0;
  m_theta_inc = (0.001 + frand()) * 0.003 * M_PI;

  int m = A_GUTS_1 + rand() % 5;
  m_mesh_instance.set_mesh( &g_mesh_list[m] );

  m_direction.set_as_angle( M_PI * 2 * frand() );

  update();
}

MeshInstance & ExplodingPart::mesh_instance() {
  return m_mesh_instance; 
}

bool ExplodingPart::active() {
  return m_count > 0;
}

void ExplodingPart::update() {

  if( m_count > 0 ) {
    m_count--;
  } else
    return;

  float ypos = m_height * fabs(sin( m_theta ));

  m_height *= 0.8;
  m_theta  += m_theta_inc;

  m_rot_alpha += m_rot_alpha_inc;
  m_rot_beta  += m_rot_beta_inc;

  m_position  += m_direction * m_speed;

  m_mesh_instance.set_rotation( m_rot_alpha, 0, m_rot_beta );
  m_mesh_instance.set_translation( m_position.to_vector3( ypos ));

  m_mesh_instance.transform(); 
}

