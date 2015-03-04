
#include <iostream>
using std::cout;
using std::endl;

#include <stdlib.h>

#include "world.hh"
#include "mesh.hh"
#include "mesh-instance.hh"
#include "draw-mesh.hh"
#include "assets.hh"
#include "application.hh"

#include "player.hh"


const float tank_heading_to_bg_offset = 407.4366;
const int horizon_level = 270;

using namespace std;

World::World() {
}

void World::spawn_obstacle( float x, float y, int id ) {

  m_obstacles.push_back( Obstacle(id) );
  
  m_obstacles.back().set_pos( x, y, 0, 0 );
}

void World::spawn_obstacle( int x, int y ) {
  spawn_obstacle( (float)x, (float)y, rand() % A_OBSTACLE10 );
}

void World::spawn_tank( MidTank &mt ) {

  int zone = -1;
  
  const Vector2 &pp = m_player_tank->position();

  // hopefully we won't spawn over another tank.
  
  for( ;; ) {
    
    if( pp.x > -20 && pp.x < 20 )
      if( pp.y > -20 && pp.y < 20 ) {
	zone = SZ_OUTER;
	break;
      }

    if( pp.x < 0 ) {
      if( pp.y < 0 ) {
	zone = SZ_BOTTOM_LEFT;
	break;
	
      } else {
	zone = SZ_TOP_LEFT;
	break;
      }
    }

    if( pp.y < 0 ) {
      zone = SZ_BOTTOM_RIGHT;
      break;
    }

    zone = SZ_TOP_RIGHT;
    break;
  }

  int max = m_spawn_zones[zone].size();
  int pos_i = m_spawn_zones[zone][rand() % max];
  mt.activate( m_baddie_spawn_point[pos_i] );
}

void World::setup( Application *a, Window &w, Camera &c, Player &p ) {

  m_app    = a;
  m_window = &w;
  m_camera = &c;
  m_player = &p;
  m_player_tank = &p.current_tank();

  m_hud_bg = &g_sprite_list[S_HUD_BG];

  spawn_obstacle( -50,  50 );
  spawn_obstacle(  50,  50 );
  spawn_obstacle( -20,  30 );
  spawn_obstacle( -20,  20 );
  spawn_obstacle(   0,  20 );
  spawn_obstacle(  10,  20 );
  spawn_obstacle(  20,  20 );
  spawn_obstacle( -50,   0 );
  spawn_obstacle( -20,   0 );
  spawn_obstacle(  20,   0 );
  spawn_obstacle(  50,   0 );
  spawn_obstacle( -20, -20 );
  spawn_obstacle( -10, -20 );
  spawn_obstacle(   0, -20 );
  spawn_obstacle( -20, -30 );
  spawn_obstacle(  20, -30 );
  spawn_obstacle(  20, -40 );
  spawn_obstacle( -50, -50 );
  spawn_obstacle(  50, -50 );
  
  m_player_tank->set_pos( 0, 0 );

  m_baddie_spawn_point.push_back( Vector2( -40,  40 ));
  m_baddie_spawn_point.push_back( Vector2(  40,  40 ));
  m_baddie_spawn_point.push_back( Vector2(  20,  30 ));
  m_baddie_spawn_point.push_back( Vector2( -10,   0 ));
  m_baddie_spawn_point.push_back( Vector2(  20, -20 ));
  m_baddie_spawn_point.push_back( Vector2( -10, -30 ));
  m_baddie_spawn_point.push_back( Vector2( -40, -40 ));
  m_baddie_spawn_point.push_back( Vector2(  40, -40 ));

  m_spawn_zones.resize(SZ_COUNT);
  m_spawn_zones[SZ_TOP_LEFT].push_back(0);
  m_spawn_zones[SZ_TOP_LEFT].push_back(2);
  m_spawn_zones[SZ_TOP_LEFT].push_back(3);
  m_spawn_zones[SZ_TOP_LEFT].push_back(4);
  m_spawn_zones[SZ_TOP_LEFT].push_back(7);
  m_spawn_zones[SZ_TOP_LEFT].push_back(8);
  
  m_spawn_zones[SZ_TOP_RIGHT].push_back(1);
  m_spawn_zones[SZ_TOP_RIGHT].push_back(4);
  m_spawn_zones[SZ_TOP_RIGHT].push_back(5);
  m_spawn_zones[SZ_TOP_RIGHT].push_back(6);
  m_spawn_zones[SZ_TOP_RIGHT].push_back(9);
  m_spawn_zones[SZ_TOP_RIGHT].push_back(10);
  
  m_spawn_zones[SZ_BOTTOM_LEFT].push_back(7);
  m_spawn_zones[SZ_BOTTOM_LEFT].push_back(8);
  m_spawn_zones[SZ_BOTTOM_LEFT].push_back(11);
  m_spawn_zones[SZ_BOTTOM_LEFT].push_back(12);
  m_spawn_zones[SZ_BOTTOM_LEFT].push_back(13);
  m_spawn_zones[SZ_BOTTOM_LEFT].push_back(14);
  m_spawn_zones[SZ_BOTTOM_LEFT].push_back(17);
  
  m_spawn_zones[SZ_BOTTOM_RIGHT].push_back(9);
  m_spawn_zones[SZ_BOTTOM_RIGHT].push_back(10);
  m_spawn_zones[SZ_BOTTOM_RIGHT].push_back(13);
  m_spawn_zones[SZ_BOTTOM_RIGHT].push_back(15);
  m_spawn_zones[SZ_BOTTOM_RIGHT].push_back(16);
  m_spawn_zones[SZ_BOTTOM_RIGHT].push_back(18);
  
  m_spawn_zones[SZ_OUTER].push_back(0);
  m_spawn_zones[SZ_OUTER].push_back(1);
  m_spawn_zones[SZ_OUTER].push_back(2);
  m_spawn_zones[SZ_OUTER].push_back(7);
  m_spawn_zones[SZ_OUTER].push_back(10);
  m_spawn_zones[SZ_OUTER].push_back(16);
  m_spawn_zones[SZ_OUTER].push_back(17);
  m_spawn_zones[SZ_OUTER].push_back(18);

  m_baddies.push_back( MidTank(this) );
}

void World::draw_hud() {

  m_hud_bg->draw( *m_window, 15, 10, 100 );

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
}

void World::do_play() {

  LineVectorSprite &logo = g_sprite_list[S_MINI_LOGO];
  LineVectorSprite &bg1  = g_sprite_list[S_BG1];
  LineVectorSprite &bg2  = g_sprite_list[S_BG2];
  LineVectorSprite &bg3  = g_sprite_list[S_BG3];
  LineVectorSprite &bg4  = g_sprite_list[S_BG4];

  LineVectorSprite &aimer        = g_sprite_list[S_CANNON_RETICULE];

  bool run_loop = true;

  bool *keys = m_window->m_keys;

  vector<Obstacle>::iterator ob_it;
  vector<MidTank>::iterator b_it;
  list<Bullet>::iterator bu_it;

  cout << "map has " << m_baddies.size() << " baddies" << endl;
  
  while( m_window->active() && run_loop ) {

    if( m_player_tank->armour() == 0 )
      break;

    m_player_tank->move( m_obstacles );

    for( b_it = m_baddies.begin(); b_it != m_baddies.end(); b_it++ )
      b_it->think_and_move( m_player_tank, m_obstacles );
  
    for( bu_it = m_bullets.begin(); bu_it != m_bullets.end(); ) {

      bu_it->move();

      if( !bu_it->is_active() ) {
        cout << "bullet expired" << endl;
        bu_it = m_bullets.erase(bu_it);
        continue;
      }

      if( bu_it->has_hit_obstacle( m_obstacles )) {
        bu_it = m_bullets.erase(bu_it);
        continue; 
      }

      if( bu_it->owner() == Bullet::B_PLAYER ) {

        MidTank *hit = bu_it->has_hit_enemy( m_baddies );
        if(hit) { 
          hit->deactivate();
	  m_player->add_score(15);
          bu_it = m_bullets.erase(bu_it);
          continue; 
        }

      } else {

        if( m_player_tank->is_hit_by( bu_it->position())) {
	  m_player_tank->do_damage(5);
          bu_it = m_bullets.erase(bu_it);
          continue; 
        } 
      }

      bu_it++;
    }

    for( b_it = m_baddies.begin(); b_it != m_baddies.end(); b_it++ )
      if( !b_it->is_active() )
	spawn_tank( *b_it );

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

    aimer.draw( *m_window, 270, 173 );

    draw_hud();

    m_window->end_raster();
    m_window->tick();

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

    if( keys[Window::K_S] ) 
      m_player_tank->raise( 0.1 );

    if( keys[Window::K_X] )
      m_player_tank->raise( -0.1 );
    
    if( keys[Window::K_Q] )
      m_player_tank->zero_y();

    if( keys[Window::K_Z] ) {
      m_player_tank->strafe( 0.1 );
    }

    if( keys[Window::K_C] ) {
      m_player_tank->strafe( -0.1 );
    }

    switch( m_window->inkey() ) {
      case Window::K_ESCAPE:
        run_loop = false;
        break;
        
	//case Window::K_TAB:
	//        m_baddies[0].reset();
        //break;
    }
  }
}

void World::do_crash() {
  
  LineVectorSprite &screen_crack = g_sprite_list[S_SCREEN_CRACK];
  int hold = 200;
  
  while( hold ) {

    hold--;

    m_window->begin_raster();
    
    draw_hud();
      
    screen_crack.draw( *m_window, 88, 80 );

    m_window->end_raster();

    m_window->tick();

  }
}

void World::do_game_over() {
  cout << "You have died. Your final score is " << m_player->current_score() << endl;
}

void World::run() {

  while(1) {
    
    do_play();

    if( m_player->tank_count() > 1 ) {
      
      do_crash();      
      m_player->take_tank();      
      
    } else {

      do_game_over();      
      break;
    }
  }
}

void World::shoot_player_bullet( const Vector2 &pos, float heading ) {
  cout << "player shoot" << endl;
  m_bullets.push_back( Bullet( pos, heading, 100, Bullet::B_PLAYER ));
}

void World::shoot_enemy_bullet( const Vector2 &pos, float heading ) {
  cout << "player shoot" << endl;
  m_bullets.push_back( Bullet( pos, heading, 100, Bullet::B_ENEMY ));
}

