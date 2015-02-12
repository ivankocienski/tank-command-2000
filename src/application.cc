
#include <SDL/SDL.h>

#include <iostream>
#include <vector>
#include <sstream>
#include <cmath>

#include "common.hh"
#include "application.hh"
#include "window.hh"
#include "mesh.hh"
#include "draw-mesh.hh"

bool g_capture = false;

using namespace std;
const int Application::XRES = 640;
const int Application::YRES = 480;

// near and far clip planes. just randomly picked
const float Application::P_NEAR = 0.25;
const float Application::P_FAR  = 20.0;
const float Application::FOV    = 90.0; // horizontal

Application::Application( int argc, char ** argv ) { 
}

int Application::main() {

  bool run_loop = true;
  char buffer[200];

  if( !m_window.open( XRES, YRES, "tc2k" )) {
    cerr << "could not open window" << endl;
    return -1;
  }

  Mesh tank_mesh;
  tank_mesh.load_from( "data/tank.vex" );

  vector<MeshInstance> mesh_instances;

  mesh_instances.push_back( MeshInstance( &tank_mesh ));

  Camera camera(m_window);
  camera.setup( FOV, P_NEAR, P_FAR );

  vector<MeshInstance>::iterator mi_it;

  float a = 0;
  float pos = 0;
  float pos_inc = 0.1;


  while( m_window.active() && run_loop ) {
  
    a += 0.1;

    pos += pos_inc;
    if( pos < -5 || pos > 5 ) pos_inc = -pos_inc;

    mesh_instances[0].set_translation( 0, pos, 0 );
    mesh_instances[0].set_rotation( 0, 0, a );
    mesh_instances[0].transform();

    m_player.look( camera );

    m_window.begin_raster();

    for( mi_it = mesh_instances.begin(); mi_it != mesh_instances.end(); mi_it++ ) {

      DrawMesh dm( *mi_it, camera );

      dm.clip_to_frustum();

      if( !dm.is_visible() ) continue;

      // TODO; translate from object space to world space

      dm.camera_transform();

      dm.draw();

      // TODO ... collect draw meshes and z-sort them and paint them backward onto screen ;)
    }

    m_window.end_raster();

    snprintf( buffer, 200, "x=%f y=%f", m_player.foot_x(), m_player.foot_y() );
    m_window.puts( 5, 5, buffer );

    m_window.tick();
    g_capture = false;

    if( m_window.m_keys[Window::K_LEFT] ) {
      m_player.turn( 0.1 );
    }

    if( m_window.m_keys[Window::K_RIGHT] ) {
      m_player.turn( -0.1 );
    }

    if( m_window.m_keys[Window::K_UP] ) {
      m_player.walk( 0.1 );
    }

    if( m_window.m_keys[Window::K_DOWN] ) {
      m_player.walk( -0.1 );
    }

    switch( m_window.inkey() ) {
      case Window::K_ESCAPE:
        run_loop = false;
        break;
    }
  }
  
  return 0;
}


