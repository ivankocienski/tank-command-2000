
#pragma once

#include <vector>

#include "mesh.hh"

enum {
  A_TANK,
  A_OBSTACLE1,
  A_OBSTACLE2,
  A_OBSTACLE3,
  A_OBSTACLE4,
  A_MAX
};

extern std::vector<Mesh> g_mesh_list;

bool load_assets();


