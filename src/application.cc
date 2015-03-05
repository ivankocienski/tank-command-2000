
#include <iostream>

#include "window.hh"
#include "player.hh"
#include "camera.hh"
#include "world.hh"
#include "common.hh"
#include "application.hh"
#include "assets.hh"
#include "draw-mesh.hh"

using namespace std;

static const int XRES = 640;
static const int YRES = 480;

// near and far clip planes. just randomly picked
static const float P_NEAR = 0.25;
static const float P_FAR  = 100.0;
static const float FOV    = 90.0; // horizontal

#include "char-table.hh"

Application::Application( int argc, char ** argv ) { 
}

void Application::do_splash(Camera &cam) {
  
  LineVectorSprite &logo = g_sprite_list[S_SPLASH_LOGO];
  MeshInstance tank_mesh(&g_mesh_list[A_MID_TANK]);

  int hold = 1000;
  float angle = 0;

  tank_mesh.set_color(1);
  tank_mesh.set_translation( 0, -1.5, -6 );

  cam.look(
	   Vector3(  0, 0,  0 ),
	   Vector3(  0, 0, -1 ),
	   Vector3( 1, 0,  0 )
	   );
	   
  while( hold && m_window.active() ) {
    hold--;

    angle += 0.02;
    
    tank_mesh.set_rotation( angle, 0, 0 );
    tank_mesh.transform();

    m_window.begin_raster();

    DrawMesh dm( tank_mesh, &cam );
    dm.clip_to_frustum();
    dm.camera_transform();
    dm.draw(); 

    logo.draw( m_window, 25, 100 );

    if( hold < 950 )
      draw_text( 25, 80, "A GAME BY IVAN KOCIENSKI 2015" );

    if( hold < 800 )
      if( (hold >> 2) & 1 ) draw_text( 280, 450, "PRESS SPACE BAR TO START" );
      
    m_window.end_raster();

    m_window.tick();

    if( m_window.inkey() == Window::K_SPACE && hold < 800 )
      break;
  }

}

int Application::main() {

  if( !m_window.open( XRES, YRES, "tc2k" )) {
    cerr << "could not open window" << endl;
    return -1;
  }

  if( !load_assets() ) {
    cerr << "could not load assets" << endl;
    return -1;
  }

  Camera camera(m_window);
  camera.setup( FOV, P_NEAR, P_FAR );

  World world;

  Player player(&world); 

  world.setup( this, m_window, camera, player );

  while(m_window.active()) {

    do_splash(camera);
    
    world.run();
  }

  return 0;
}

void Application::draw_text(int x, int y, const char *c ) {
  
  while( *c ) {
    if( char_table[*c] != -1 ) {
      g_sprite_list[ char_table[*c] ].draw( m_window, x, y );
    }

    c++;
    x += 6;
  }
}

static int fudgex[10] = { 3, 12, 3, 4, 3, 3, 3, 3, 3, 3 };
static int fudgey[10] = { 0,  1, 0, 0, 1, 0, 1, 0, 0, 0 };

void Application::draw_hud_number(int x, int y, unsigned int n ) {
  
  int sprite;
  int d;

  x -= 12;

  if( !n ) {
    g_sprite_list[ S_HUD_NUM_0 ].draw( m_window, x + fudgex[0], y + fudgey[0] );
    return; 
  }

  while( n ) {

    d = n % 10;
    sprite = d + S_HUD_NUM_0;

    g_sprite_list[ sprite ].draw( m_window, x + fudgex[d], y + fudgey[d] );

    n /= 10;
    x -= 14;
  }
}
