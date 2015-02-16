
#pragma once

#include <vector>

#include "window.hh"
#include "player.hh"
#include "camera.hh"
#include "mesh-instance.hh"

class World {
private:

  Window *m_window;
  Camera *m_camera;
  Player *m_player;
  PlayerTank *m_player_tank;

  std::vector<MeshInstance> m_mesh_instances;

  void spawn_obstacle( float, float, int );

public:

  World();

  void setup( Window&, Camera&, Player& );

  void run();
};

