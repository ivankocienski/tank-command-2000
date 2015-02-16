
#include "world.hh"

#include "math/vector3.hh"
#include "mesh.hh"
#include "draw-mesh.hh"
#include "assets.hh"

#include "line-vector-sprite.hh"

const float tank_heading_to_bg_offset = 407.4366;
const int horizon_level = 250;

using namespace std;

World::World() { 
}

void World::spawn_obstacle( float x, float y, int id ) {

  m_mesh_instances.push_back( MeshInstance( &g_mesh_list[id]));
  
  MeshInstance &mi( m_mesh_instances.back() );

  mi.set_translation( x, 0, y );
  mi.transform();
}

void World::setup( Window &w, Camera &c, Player &p ) {

  m_window = &w;
  m_camera = &c;
  m_player = &p;
  m_player_tank = &p.current_tank();

  spawn_obstacle(  1, 1, 1 );
  spawn_obstacle(  5, 1, 2 );
  spawn_obstacle( 10, 5, 3 );
  spawn_obstacle(  4, 5, 4 );

  m_player_tank->set_pos( -3, -3 );
}

void World::run() {

  LineVectorSprite &logo = g_sprite_list[S_MINI_LOGO];
  LineVectorSprite &bg1  = g_sprite_list[S_BG1];
  LineVectorSprite &bg2  = g_sprite_list[S_BG2];
  LineVectorSprite &bg3  = g_sprite_list[S_BG3];
  LineVectorSprite &bg4  = g_sprite_list[S_BG4];

  bool run_loop = true;

  bool *keys = m_window->m_keys;

  vector<MeshInstance>::iterator mi_it;
  void set_pos( float, float, float );
  
  while( m_window->active() && run_loop ) {

    m_player_tank->move( m_mesh_instances );
  
    m_player_tank->look( m_camera );

    m_window->begin_raster();

    int bgo = 2560 - (tank_heading_to_bg_offset * m_player_tank->heading());

    bg1.draw( *m_window,    0 - bgo, horizon_level - 80 );
    bg2.draw( *m_window,  640 - bgo, horizon_level - 80);
    bg3.draw( *m_window, 1280 - bgo, horizon_level - 65);
    bg4.draw( *m_window, 1920 - bgo, horizon_level - 45);
    bg1.draw( *m_window, 2560 - bgo, horizon_level - 80);



    for( mi_it = m_mesh_instances.begin(); mi_it != m_mesh_instances.end(); mi_it++ ) {

      DrawMesh dm( *mi_it, m_camera );

      dm.clip_to_frustum();

      if( !dm.is_visible() ) continue;

      // TODO; translate from object space to world space

      dm.camera_transform();

      dm.draw();

      // TODO ... collect draw meshes and z-sort them and paint them backward onto screen ;)
    }

    logo.draw( *m_window, 10, 10 );

    m_window->end_raster();

    //snprintf( buffer, 200, "x=%f y=%f", m_player.foot_x(), m_player.foot_y() );
    //m_window->puts( 5, 5, buffer );


    m_window->tick();
    //g_capture = false;


    if( keys[Window::K_LEFT] ) {
      m_player_tank->turn( 0.1 );
    }

    if( keys[Window::K_RIGHT] ) {
      m_player_tank->turn( -0.1 );
    }

    if( keys[Window::K_UP] ) {
      m_player_tank->walk( 0.1 );
    }

    if( keys[Window::K_DOWN] ) {
      m_player_tank->walk( -0.1 );
    }

    if( keys[Window::K_A] ) {
      m_player_tank->strafe( 0.1 );
    }

    if( keys[Window::K_D] ) {
      m_player_tank->strafe( -0.1 );
    }

    switch( m_window->inkey() ) {
      case Window::K_ESCAPE:
        run_loop = false;
        break;
    }
  }
}
