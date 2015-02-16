
#pragma once

#include <vector>

#include "mesh.hh"
#include "line-vector-sprite.hh"

enum {
  A_TANK,
  A_OBSTACLE1,
  A_OBSTACLE2,
  A_OBSTACLE3,
  A_OBSTACLE4,
  A_MAX
};

enum {
  S_MINI_LOGO,
  S_BG1,
  S_BG2,
  S_BG3,
  S_BG4,

  S_MAX
};

extern std::vector<Mesh> g_mesh_list;
extern std::vector<LineVectorSprite> g_sprite_list;

bool load_assets();


