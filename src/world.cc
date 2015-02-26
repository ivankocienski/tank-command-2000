
#include <iostream>
using std::cout;
using std::endl;

#include "world.hh"
#include "mesh.hh"
#include "mesh-instance.hh"
#include "draw-mesh.hh"
#include "assets.hh"
#include "application.hh"

#include "player.hh"

#include "line-vector-sprite.hh"

const float tank_heading_to_bg_offset = 407.4366;
const int horizon_level = 270;

using namespace std;

World::World() { 
}

void World::spawn_obstacle( float x, float y, int id ) {

  m_obstacles.push_back( Obstacle(id) );
  
  m_obstacles.back().set_pos( x, y, 0, 0 );
}

void World::spawn_tank( float x, float y ) {
  
  m_baddies.push_back( MidTank() );
  MidTank &mi( m_baddies.back() );

  mi.set_pos( x, y );
}

void World::setup( Application *a, Window &w, Camera &c, Player &p ) {

  m_app    = a;
  m_window = &w;
  m_camera = &c;
  m_player = &p;
  m_player_tank = &p.current_tank();

  spawn_obstacle(  1, 1, 0 );
  spawn_obstacle(  5, 1, 1 );
  spawn_obstacle( 10, 5, 2 );
  spawn_obstacle(  4, 5, 3 );

  m_player_tank->set_pos( -3, -3 );

  spawn_tank( 5, 10 );
}

void World::run() {

  LineVectorSprite &logo = g_sprite_list[S_MINI_LOGO];
  LineVectorSprite &bg1  = g_sprite_list[S_BG1];
  LineVectorSprite &bg2  = g_sprite_list[S_BG2];
  LineVectorSprite &bg3  = g_sprite_list[S_BG3];
  LineVectorSprite &bg4  = g_sprite_list[S_BG4];

  LineVectorSprite &aimer        = g_sprite_list[S_CANNON_RETICULE];
  LineVectorSprite &hud_bg       = g_sprite_list[S_HUD_BG];
  LineVectorSprite &screen_crack = g_sprite_list[S_SCREEN_CRACK];

  bool run_loop = true;

  bool *keys = m_window->m_keys;

  vector<Obstacle>::iterator ob_it;
  vector<MidTank>::iterator b_it;
  list<Bullet>::iterator bu_it;
  
  while( m_window->active() && run_loop ) {

    m_player_tank->move( m_obstacles );

    //for( b_it = m_baddies.begin(); b_it != m_baddies.end(); b_it++ )
    //  b_it->think_and_move( m_player_tank, m_obstacles );
  
    for( bu_it = m_bullets.begin(); bu_it != m_bullets.end(); ) {

      bu_it->move();

      if( !bu_it->is_active() ) {
        cout << "bullet expired" << endl;
        bu_it = m_bullets.erase(bu_it);
        continue;
      }

      if( bu_it->has_hit_obstacle( m_obstacles )) {
        cout << "bullet hit obstacle" << endl;
        bu_it = m_bullets.erase(bu_it);
        continue; 
      }

      MidTank *hit = bu_it->has_hit_enemy( m_baddies );
      if(hit) { 
        cout << "bullet hit baddie" << endl;
        //hit->deactivate();
        bu_it = m_bullets.erase(bu_it);
        continue; 
      }

      bu_it++;
    }

    m_player_tank->look( m_camera );


    m_window->begin_raster();

    int bgo = 2560 - (tank_heading_to_bg_offset * m_player_tank->heading());

    bg1.draw( *m_window,    0 - bgo, horizon_level - 80, 100 );
    bg2.draw( *m_window,  640 - bgo, horizon_level - 80, 100 );
    bg3.draw( *m_window, 1280 - bgo, horizon_level - 65, 100 );
    bg4.draw( *m_window, 1920 - bgo, horizon_level - 45, 100 );
    bg1.draw( *m_window, 2560 - bgo, horizon_level - 80, 100 );


    // TODO ... collect draw meshes and z-sort them and paint them backward onto screen ;)

    for( ob_it = m_obstacles.begin(); ob_it != m_obstacles.end(); ob_it++ ) {

      DrawMesh dm( ob_it->mesh(), m_camera );

      dm.clip_to_frustum();

      if( !dm.is_visible() ) continue;

      dm.camera_transform();

      dm.draw(); 
    }

    for( b_it = m_baddies.begin(); b_it != m_baddies.end(); b_it++ ) {
      if( !b_it->is_active() ) continue;

      DrawMesh dm( b_it->mesh_instance(), m_camera );

      dm.clip_to_frustum();

      if( !dm.is_visible() ) continue;

      dm.camera_transform();

      dm.draw(); 
    }

    for( bu_it = m_bullets.begin(); bu_it != m_bullets.end(); bu_it++ ) {

      DrawMesh dm( bu_it->mesh(), m_camera );

      dm.clip_to_frustum();

      if( !dm.is_visible() ) continue;

      dm.camera_transform();

      dm.draw(); 
    }

    if( m_player_tank->armour() > 0 ) 
      aimer.draw( *m_window, 270, 173 );
    else
      screen_crack.draw( *m_window, 88, 80 );

    hud_bg.draw( *m_window, 15, 10, 100 );

    // score
    m_app->draw_hud_number( 94, 45, m_player->current_score() );

    // tanks
    m_app->draw_hud_number( 625, 11, m_player->tank_count() );

    // armour
    m_app->draw_hud_number( 625, 46, m_player_tank->armour() );

    // hud
    //   radar
    //   lives
    //   damage
    //   ammo
    //     shells
    //     ATG (anti tank guns)
    //     missiles
    //   score

//    m_window->draw_line( 100, 100, 420, 100, 2 );
//    m_window->draw_line( 100, 100, 100, 300, 2 );
//    m_window->draw_line( 420, 100, 420, 300, 2 );
//    m_window->draw_line( 100, 300, 420, 300, 2 );

    //m_app->draw_text( 20, 50, "ABCD 1234   HELLO WORLD" );

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

    if( keys[Window::K_SPACE ]) {
      m_player_tank->fire( true );

    } else {
      m_player_tank->fire( false );
    }

//    if( keys[Window::K_A] ) {
//      m_player_tank->strafe( 0.1 );
//    }
//
//    if( keys[Window::K_D] ) {
//      m_player_tank->strafe( -0.1 );
//    }

    switch( m_window->inkey() ) {
      case Window::K_ESCAPE:
        run_loop = false;
        break;
    }
  }
}

void World::shoot_bullet( Vector2 &pos, float heading ) {
  cout << "player shoot" << endl;
  m_bullets.push_back( Bullet( pos, heading, 100 ));
}

// debug bullet rotation diggity
//Bullet*
