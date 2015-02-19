
#pragma once

#include <vector>

#include "window.hh"
#include "player.hh"
#include "camera.hh"
#include "mesh-instance.hh"

#include "baddies/mid-tank.hh"

class World {
private:

  Window *m_window;
  Camera *m_camera;
  Player *m_player;
  PlayerTank *m_player_tank;

  std::vector<MeshInstance> m_mesh_instances;
  std::vector<MidTank> m_baddies;

  void spawn_obstacle( float, float, int );
  void spawn_tank( float, float );
  
public:

  World();

  void setup( Window&, Camera&, Player& );

  void run();
};

