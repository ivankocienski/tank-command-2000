
#pragma once

#include <vector>

#include "window.hh"
#include "camera.hh"

#include "player.hh"
#include "obstacle.hh" 
#include "baddies/mid-tank.hh"

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

  void spawn_obstacle( float, float, int );
  void spawn_tank( float, float );
  
public:

  World();

  void setup( Application*, Window&, Camera&, Player& );

  void run();
};

