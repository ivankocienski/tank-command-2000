
#pragma once

#include <vector>
#include <list>

#include "window.hh"
#include "camera.hh"

#include "obstacle.hh" 
#include "baddies/mid-tank.hh"
#include "bullet.hh"

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
  
  void spawn_obstacle( float, float, int );
  void spawn_tank( float, float );
  
public:

  World();

  void setup( Application*, Window&, Camera&, Player& );

  void shoot_player_bullet( const Vector2&, float );
  void shoot_enemy_bullet( const Vector2&, float );

  Bullet* first_bullet();

  void run();
};

