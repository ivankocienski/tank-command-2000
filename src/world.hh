
#pragma once

#include <vector>
#include <list>

#include "window.hh"
#include "camera.hh"
#include "math/vector2.hh"

#include "obstacle.hh" 
#include "baddies/mid-tank.hh"
#include "bullet.hh"
#include "line-vector-sprite.hh"

class Player;
class PlayerTank;
class Application;

class World {
private:

  Application *m_app;
  Window *m_window;
  Camera *m_camera;
  Player *m_player;
  PlayerTank *m_player_tank;

  std::vector<Obstacle> m_obstacles;
  std::vector<MidTank> m_baddies;
  std::list<Bullet> m_bullets;

  std::vector<Vector2> m_baddie_spawn_point;
  std::vector< std::vector<int> > m_spawn_zones;

  LineVectorSprite *m_hud_bg;

  enum { // spawn zone
    SZ_TOP_LEFT,
    SZ_TOP_RIGHT,
    SZ_BOTTOM_LEFT,
    SZ_BOTTOM_RIGHT,
    SZ_OUTER,
    SZ_COUNT
  };

  enum {
    PO_QUIT,
    PO_DIED
  };

  void spawn_obstacle( float, float, int );
  void spawn_obstacle( int, int );
  void spawn_tank( MidTank& );

  void draw_hud(unsigned char);
  int  do_play();
  void do_crash();
  void do_game_over();

public:

  World();

  void setup( Application*, Window&, Camera&, Player& );

  void shoot_player_bullet( const Vector2&, float );
  void shoot_enemy_bullet( const Vector2&, float );

  Bullet* first_bullet();

  void run();
};

