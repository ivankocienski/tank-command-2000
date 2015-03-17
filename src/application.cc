
#include <iostream>
#include <stdlib.h>
#include <time.h>

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
#include "help-text.hh"

Application::Application( int argc, char ** argv ) { 
}

void Application::do_help() {

  while( m_window.active() ) {

    const char **text = help_text;
    int ypos = 20;

    while( *text ) {

      draw_text( 25, ypos, *text );

      text++;
      ypos += 30;
    }

    m_window.tick();

    if( m_window.inkey() == Window::K_SPACE ) return;
  }
  

}

int Application::do_menu(Camera &cam) {
  
  int menu_opt = 0;

  LineVectorSprite &logo = g_sprite_list[S_SPLASH_LOGO];
  MeshInstance tank_mesh(&g_mesh_list[A_MID_TANK]);

  int hold = 1000;
  float angle = 0;
  int xpos = 180;
  float yoffs = 300;


  tank_mesh.set_color(15);
  tank_mesh.set_translation( 0, -0.5, -5 );

  cam.look(
	   Vector3(  0, 0,  0 ),
	   Vector3(  0, 0, -1 ),
	   Vector3( 1, 0,  0 )
	   );
	   
  while( hold && m_window.active() ) {
    hold--;

    yoffs *= 0.95;
    angle += 0.02;
    
    tank_mesh.set_rotation( angle, 0, 0 );
    tank_mesh.transform();

    DrawMesh dm( &tank_mesh, &cam );
    dm.clip_to_frustum();
    dm.camera_transform();
    dm.draw(); 

    logo.draw( m_window, 25, 50 );

    draw_text( 25, 20, "BY IVAN KOCIENSKI 2015" );

    if( menu_opt == 0 ) {
      if( (hold >> 2) & 1 ) 
        draw_text( xpos, yoffs + 200, "START GAME" );

    } else
      draw_text( xpos, yoffs + 200, "START GAME" );

    if( menu_opt == 1 ) {
      if( (hold >> 2) & 1 ) 
        draw_text( xpos, yoffs + 240, "HELP" );

    } else
      draw_text( xpos, yoffs + 240, "HELP" );

    if( menu_opt == 2 ) {
      if( (hold >> 2) & 1 ) 
        draw_text( xpos, yoffs + 280, "QUIT TO OS" );

    } else
      draw_text( xpos, yoffs + 280, "QUIT TO OS" );
      
    m_window.tick();

    int key = m_window.inkey();

    if( key == Window::K_UP ) {
      menu_opt--;
      if( menu_opt < 0 ) menu_opt = 2;
    }

    if( key == Window::K_DOWN ) {
      menu_opt++;
      if( menu_opt > 2 ) menu_opt = 0;
    }

    if( key == Window::K_ENTER ) return menu_opt; 
  }

  return -1;
}

int Application::main() {

  srand(time(NULL));

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

    switch( do_menu(camera)) {
      case MO_PLAY:
        world.run();
        break;

      case MO_HELP:
        do_help();
        break;

      case MO_QUIT:
        return 0;
        break;
    }
  }

  return 0;
}

void Application::draw_text(int x, int y, const char *c ) {
  
  while( *c ) {
    if( char_table[*c] != -1 ) {
      g_sprite_list[ char_table[*c] ].draw( m_window, x, y, 143 );
    }

    c++;
    x += 19;
  }
}

static int fudgex[10] = { 3, 12, 3, 4, 3, 3, 3, 3, 3, 3 };
static int fudgey[10] = { 0,  1, 0, 0, 1, 0, 1, 0, 0, 0 };

void Application::draw_hud_number(int x, int y, unsigned int n ) {
  
  int sprite;
  int d;

  x -= 12;

  if( !n ) {
    g_sprite_list[ S_HUD_NUM_0 ].draw( m_window, x + fudgex[0], y + fudgey[0], 29 );
    return; 
  }

  while( n ) {

    d = n % 10;
    sprite = d + S_HUD_NUM_0;

    g_sprite_list[ sprite ].draw( m_window, x + fudgex[d], y + fudgey[d], 29 );

    n /= 10;
    x -= 14;
  }
}
