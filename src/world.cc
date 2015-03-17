
#include <iostream>
using std::cout;
using std::endl;

#include <algorithm>

#include <stdlib.h>

#include "math/operations.hh"

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

static int draw_mesh_cmp( const DrawMesh& a, const DrawMesh &b ) {

  return a.camera_distance() > b.camera_distance();
}

void World::spawn_obstacle( float x, float y, int id ) {

  m_obstacles.push_back( Obstacle(id) );

  m_obstacles.back().set_pos( x, y, 0, 0 );
}

void World::spawn_obstacle( int x, int y ) {
  spawn_obstacle( (float)x, (float)y, rand() % A_OBSTACLE10 );
}

const char *zone_names [] = {
  "SZ_TOP_LEFT",
  "SZ_TOP_RIGHT",
  "SZ_BOTTOM_LEFT",
  "SZ_BOTTOM_RIGHT",
  "SZ_OUTER"
};

void World::spawn_tank( MidTank *mt ) {

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
        zone = SZ_TOP_RIGHT;
        break; 
      }
      
      zone = SZ_BOTTOM_RIGHT;
      break; 
    }

    if( pp.y < 0 ) {
      zone = SZ_TOP_LEFT;
      break;
    }

    zone = SZ_BOTTOM_LEFT;
    break;
  }

  int max = m_spawn_zones[zone].size();

  cout 
    << "SPAWN TANK: zone=" 
    << zone_names[zone] 
    << " (" 
    << zone 
    << ") max=" 
    << max 
    << endl;

  int pos_i = m_spawn_zones[zone][rand() % max];

  cout 
    << "  i=" << pos_i
    << " x=" << m_baddie_spawn_point[pos_i].x
    << " y=" << m_baddie_spawn_point[pos_i].y
    << endl;

  mt->activate( m_baddie_spawn_point[pos_i], m_player->difficulty() );
}

void World::spawn_powerup( const Vector2 &p ) {
  m_powerups.push_back( Powerup( p, rand() % 2 ));
}

void World::spawn_powerup( ) {

  int pi = rand() % m_baddie_spawn_point.size();

  spawn_powerup( m_baddie_spawn_point[pi] );
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


  m_baddie_spawn_point.push_back( Vector2( -40,  40 )); // 0
  m_baddie_spawn_point.push_back( Vector2(  40,  40 )); // 1
  m_baddie_spawn_point.push_back( Vector2(  20,  30 )); // 2
  m_baddie_spawn_point.push_back( Vector2( -10,  20 )); // 3

  m_baddie_spawn_point.push_back( Vector2(  10,  10 )); // 4
  m_baddie_spawn_point.push_back( Vector2( -10, -10 )); // 5

  m_baddie_spawn_point.push_back( Vector2(  20, -20 )); // 6
  m_baddie_spawn_point.push_back( Vector2( -10, -30 )); // 7
  m_baddie_spawn_point.push_back( Vector2( -40, -40 )); // 8
  m_baddie_spawn_point.push_back( Vector2(  40, -40 )); // 9

  m_spawn_zones.resize(SZ_COUNT);
  m_spawn_zones[SZ_TOP_LEFT].push_back(0);
  m_spawn_zones[SZ_TOP_LEFT].push_back(3);

  m_spawn_zones[SZ_TOP_RIGHT].push_back(1);
  m_spawn_zones[SZ_TOP_RIGHT].push_back(2);
  m_spawn_zones[SZ_TOP_RIGHT].push_back(4);

  m_spawn_zones[SZ_BOTTOM_LEFT].push_back(5);
  m_spawn_zones[SZ_BOTTOM_LEFT].push_back(7);
  m_spawn_zones[SZ_BOTTOM_LEFT].push_back(8);

  m_spawn_zones[SZ_BOTTOM_RIGHT].push_back(6);
  m_spawn_zones[SZ_BOTTOM_RIGHT].push_back(9);

  m_spawn_zones[SZ_OUTER].push_back(0);
  m_spawn_zones[SZ_OUTER].push_back(1);
  m_spawn_zones[SZ_OUTER].push_back(2);
  m_spawn_zones[SZ_OUTER].push_back(3);
  m_spawn_zones[SZ_OUTER].push_back(7);
  m_spawn_zones[SZ_OUTER].push_back(8);
  m_spawn_zones[SZ_OUTER].push_back(9);

  m_baddies.push_back( MidTank(this) );
}

void World::draw_hud( unsigned char anim_count ) {

  m_hud_bg->draw( *m_window, 15, 10, 22 );

  // score
  m_app->draw_hud_number( 94, 45, m_player->current_score() );

  // tanks
  m_app->draw_hud_number( 625, 11, m_player->tank_count() );

  // armour
  m_app->draw_hud_number( 625, 46, m_player_tank->armour() );

  // radar
  m_window->draw_pixel( 270,  0, 25 );
  m_window->draw_pixel( 370,  0, 25 );
  m_window->draw_pixel( 270, 79, 25 );
  m_window->draw_pixel( 370, 79, 25 );

  m_window->draw_pixel2( 319, 39, 63 );

  // obstacles
  vector<Obstacle>::iterator ob_it;

  Matrix3 &piv( m_player_tank->inv_model_matrix());
  Vector2 p;

  for( ob_it = m_obstacles.begin(); ob_it != m_obstacles.end(); ob_it++ ) {

    vec2_mat3_multiply( p, ob_it->position(), piv );

    float px = p.y;
    float py = p.x;

    if( px < -49 || px > 49 ) continue;
    if( py < -39 || py > 39 ) continue;

    m_window->draw_pixel( 320 + px, 40 + py, 25 );
  }

  // enemies
  if( (anim_count >> 2) & 1 ) {

    vector<MidTank>::iterator b_it;

    for( b_it = m_baddies.begin(); b_it != m_baddies.end(); b_it++ ) {

      vec2_mat3_multiply( p, b_it->position(), piv );

      float px = p.y;
      float py = p.x;

      if( px < -49 || px > 49 ) continue;
      if( py < -39 || py > 39 ) continue;

      m_window->draw_pixel2( 320 + px, 40 + py, 31 );

    }
  }

  // powerups
  if( (anim_count >> 2) & 1 ) {

    list<Powerup>::iterator pu_it;

    for( pu_it = m_powerups.begin(); pu_it != m_powerups.end(); pu_it++ ) {

      vec2_mat3_multiply( p, pu_it->position(), piv );

      float px = p.y;
      float py = p.x;

      if( px < -49 || px > 49 ) continue;
      if( py < -39 || py > 39 ) continue;

      m_window->draw_pixel2( 320 + px, 40 + py, 111 );

    }
  }

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

int World::do_play() {

  LineVectorSprite &logo  = g_sprite_list[S_MINI_LOGO];
  LineVectorSprite &bg1   = g_sprite_list[S_BG1];
  LineVectorSprite &bg2   = g_sprite_list[S_BG2];
  LineVectorSprite &bg3   = g_sprite_list[S_BG3];
  LineVectorSprite &bg4   = g_sprite_list[S_BG4];

  LineVectorSprite &aimer = g_sprite_list[S_CANNON_RETICULE];

  bool is_paused = false;
  unsigned char anim_count = 0;
  bool *keys = m_window->m_keys;

  vector<Obstacle>::iterator    ob_it;
  vector<MidTank>::iterator     b_it;
  list<Bullet>::iterator        bu_it;
  list<ExplodingPart>::iterator ep_it;
  list<Powerup>::iterator       pu_it;

  vector<DrawMesh> draw_mesh_list;
  draw_mesh_list.reserve( 200 ); // MAGIC

  m_powerup_coundown = 100;
  m_powerup_delay    = 300;

  m_player_tank->set_pos( 0, 0 );

  for( b_it = m_baddies.begin(); b_it != m_baddies.end(); b_it++ )
    spawn_tank( &(*b_it) );

  cout << "map has " << m_baddies.size() << " baddies" << endl;

  while( m_window->active() ) {

    anim_count++;

    if( m_player_tank->armour() == 0 )
      return PO_DIED;

    if( !is_paused ) {

      m_player_tank->move( m_obstacles );

      for( ep_it = m_exploding_parts.begin(); ep_it != m_exploding_parts.end(); ) {

        ep_it->update();

        if(ep_it->active()) {
          ep_it++;
        } else
          ep_it = m_exploding_parts.erase(ep_it); 
      }

      for( pu_it = m_powerups.begin(); pu_it != m_powerups.end(); ) {

        pu_it->update();

        if( pu_it->is_active() ) {

          if( m_player_tank->is_touching( pu_it->position())) {

            switch( pu_it->type() ) {
              case Powerup::PT_SCORE: m_player->add_score(50); break;
              case Powerup::PT_ARMOUR: m_player_tank->add_armour(5); break;
            }

            pu_it = m_powerups.erase( pu_it );
            continue; 
          }

          pu_it++;

        } else
          pu_it = m_powerups.erase( pu_it );
      }

      if( m_powerup_coundown == 0 ) {
        spawn_powerup();
        m_powerup_coundown = m_powerup_delay;

      } else
        m_powerup_coundown--;

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
            hit->give_damage( bu_it->damage_factor() );

            if( hit->is_active() ) {
              m_player->add_score(5);

            } else {
              make_boom(hit->position());
              spawn_powerup( hit->position());

              m_player->add_score(15);
              m_player->add_kill();
              spawn_tank( hit );
            }

            bu_it = m_bullets.erase(bu_it);
            continue; 
          }

        } else {

          if( m_player_tank->is_touching( bu_it->position())) {
            m_player_tank->do_damage(bu_it->damage_factor());
            bu_it = m_bullets.erase(bu_it);
            continue; 
          } 
        }

        bu_it++;
      }

      //for( b_it = m_baddies.begin(); b_it != m_baddies.end(); b_it++ )
      //  if( !b_it->is_active() )
      //    spawn_tank( *b_it );
    }

    m_player_tank->look( m_camera );

    int bgo = 2560 - (tank_heading_to_bg_offset * m_player_tank->heading());

    bg1.draw( *m_window,    0 - bgo, horizon_level - 80, 132 );
    bg2.draw( *m_window,  640 - bgo, horizon_level - 80, 132 );
    bg3.draw( *m_window, 1280 - bgo, horizon_level - 65, 132 );
    bg4.draw( *m_window, 1920 - bgo, horizon_level - 45, 132 );
    bg1.draw( *m_window, 2560 - bgo, horizon_level - 80, 132 );

    draw_mesh_list.clear();

    for( ob_it = m_obstacles.begin(); ob_it != m_obstacles.end(); ob_it++ ) {

      DrawMesh dm( &(ob_it->mesh()), m_camera );

      dm.clip_to_frustum();
      if( !dm.is_visible() ) continue;

      dm.camera_transform();
      draw_mesh_list.push_back( dm );
    }

    for( pu_it = m_powerups.begin(); pu_it != m_powerups.end(); pu_it++ ) {
      DrawMesh dm( &(pu_it->mesh_instance()), m_camera );

      dm.clip_to_frustum();
      if( !dm.is_visible() ) continue;

      dm.camera_transform();
      draw_mesh_list.push_back( dm ); 
    }

    for( ep_it = m_exploding_parts.begin(); ep_it != m_exploding_parts.end(); ep_it++ ) {
      DrawMesh dm( &(ep_it->mesh_instance()), m_camera );

      dm.clip_to_frustum();
      if( !dm.is_visible() ) continue;

      dm.camera_transform();
      draw_mesh_list.push_back( dm );
    }

    for( b_it = m_baddies.begin(); b_it != m_baddies.end(); b_it++ ) {
      if( !b_it->is_active() ) continue;

      DrawMesh dm( &(b_it->mesh_instance()), m_camera );

      dm.clip_to_frustum();
      if( !dm.is_visible() ) continue;

      dm.camera_transform();
      draw_mesh_list.push_back( dm );
    }

    for( bu_it = m_bullets.begin(); bu_it != m_bullets.end(); bu_it++ ) {

      DrawMesh dm( &(bu_it->mesh()), m_camera );

      dm.clip_to_frustum();
      if( !dm.is_visible() ) continue;

      dm.camera_transform();
      draw_mesh_list.push_back( dm );
    }

    sort( draw_mesh_list.begin(), draw_mesh_list.end(), draw_mesh_cmp );

    for( vector<DrawMesh>::iterator d_it = draw_mesh_list.begin(); d_it != draw_mesh_list.end(); d_it++ )
      d_it->draw();

    aimer.draw( *m_window, 270, 173, 30 );

    draw_hud(anim_count);

    m_player_tank->draw_cracks(m_window);


    if( is_paused )
      if( (anim_count >> 2) & 1 ) m_app->draw_text( 263, 250, "PAUSED" );

    m_window->tick();

    if( !is_paused ) {
      if( keys[Window::K_LEFT] ) {
        m_player_tank->want_turn( 1 );
      }

      if( keys[Window::K_RIGHT] ) {
        m_player_tank->want_turn( -1 );
      }

      if( keys[Window::K_UP] ) {
        m_player_tank->want_walk( 1 );
      }

      if( keys[Window::K_DOWN] ) {
        m_player_tank->want_walk( -1 );
      }

      if( keys[Window::K_SPACE ]) {
        m_player_tank->fire( true );

      } else {
        m_player_tank->fire( false );
      }
    }
    // if( keys[Window::K_S] ) 
    //   m_player_tank->raise( 0.1 );

    // if( keys[Window::K_X] )
    //   m_player_tank->raise( -0.1 );

    // if( keys[Window::K_Q] )
    //   m_player_tank->zero_y();

    // if( keys[Window::K_Z] ) {
    //   m_player_tank->strafe( 0.1 );
    // }

    // if( keys[Window::K_C] ) {
    //   m_player_tank->strafe( -0.1 );
    // }

    switch( m_window->inkey() ) {
      case Window::K_TAB:
        //spawn_tank( m_baddies.front() );
        //make_boom( Vector2() );
        spawn_powerup();
        break;

      case Window::K_ESCAPE:
        is_paused = !is_paused;
        break;
    }
  }

  return PO_QUIT;
}

void World::do_crash() {

  LineVectorSprite &screen_crack = g_sprite_list[S_SCREEN_CRACK];
  int hold = 200;

  while( hold ) {

    hold--;

    draw_hud(255);

    m_player_tank->draw_cracks( m_window );
    screen_crack.draw( *m_window, 88, 80 );

    m_window->tick();

  }
}

void World::do_game_over() {

  char buffer[50];
  int pos;

  snprintf( buffer, 50, "Your final score is %d", m_player->current_score() );
  pos = (640 - (19 * strlen(buffer))) / 2;

  int hold = 1000;

  while( hold ) {

    hold--;

    m_app->draw_text( 263, 100, "GAME OVER" );
    m_app->draw_text( pos, 200, buffer );

    if( hold < 500 )
      m_app->draw_text( 177, 300, "PRESS SPACE BAR" );

    m_window->tick();

    if( m_window->inkey() == Window::K_SPACE ) break;
  }
}

void World::run() {

  while(m_window->active()) {

    if( do_play() == PO_QUIT ) break;

    do_crash();

    m_player->take_tank();      

    if( m_player->tank_count() ) continue;

    do_game_over();      
    break;
  }
}

void World::shoot_player_bullet( const Vector2 &pos, float heading ) {
  cout << "player shoot" << endl;
  m_bullets.push_back( Bullet( pos, heading, 100, Bullet::B_PLAYER, 5 ));
}

void World::shoot_enemy_bullet( const Vector2 &pos, float heading ) {
  cout << "player shoot" << endl;
  m_bullets.push_back( Bullet( pos, heading, 100, Bullet::B_ENEMY, 5 ));
}

void World::make_boom( const Vector2& pos ) {

  int c = 2 + rand() % 8;

  for( int i = 0; i < c; i++ ) {
    m_exploding_parts.push_back( ExplodingPart() );

    ExplodingPart &ep = m_exploding_parts.back();
    ep.init(pos);
  }

}
