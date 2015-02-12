
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
const float Application::screen_ratio = (float)XRES / (float)YRES;

// near and far clip planes. just randomly picked
const float Application::p_near = 0.25;
const float Application::p_far  = 100.0;

const float Application::fov = 90.0; // horizontal

Application::Application( int argc, char ** argv ) { 
}

int Application::main() {

  bool run_loop = true;
  char buffer[200];

  if( !m_window.open( XRES, YRES, "wireframe demo" )) {
    cerr << "could not open window" << endl;
    return -1;
  }

  int c = 0;

  vector<Mesh> mesh_list;
  //for(int y = -5; y < 5; y++ ) 
  {
    int y = 0;

    for(int x = -5; x < 5; x++ ) {
   
      //int x = 0;
    
      mesh_list.push_back( Mesh( x, y, 1 + c ));

      c = (c+ 1 ) % 8;
    }
  }

  Camera camera(m_window);
  camera.setup( 90, XRES, YRES, 0.25, 5 );


  while( m_window.active() && run_loop ) {
    
    m_window.begin_raster();
    
    m_player.look( camera );

    Matrix4 p_trans_matrix = camera.translation_matrix();

    if(g_capture)
      cout << endl << "**** iterate ****" << endl;

    for( vector<Mesh>::iterator mesh = mesh_list.begin(); mesh != mesh_list.end(); mesh++ ) {

      DrawMesh dm( *mesh, camera );

      dm.cull_non_facing();
      
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

      case Window::K_TAB:
        cout << "capture" << endl;
        g_capture = true;
        break;
    }
  }
  
  return 0;
}


