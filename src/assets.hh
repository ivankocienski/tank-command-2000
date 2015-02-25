
#pragma once

#include <vector>

#include "mesh.hh"
#include "line-vector-sprite.hh"

enum {
  A_OBSTACLE1,
  A_OBSTACLE2,
  A_OBSTACLE3,
  A_OBSTACLE4,
  A_MID_TANK,
  A_MAX
};

enum {
  S_MINI_LOGO,
  S_BG1,
  S_BG2,
  S_BG3,
  S_BG4,

  S_CANNON_RETICULE,
  S_HUD_BG,

  S_HUD_NUM_0,
  S_HUD_NUM_1,
  S_HUD_NUM_2,
  S_HUD_NUM_3,
  S_HUD_NUM_4,
  S_HUD_NUM_5,
  S_HUD_NUM_6,
  S_HUD_NUM_7,
  S_HUD_NUM_8,
  S_HUD_NUM_9,

  S_CHAR_0,
  S_CHAR_1,
  S_CHAR_2,
  S_CHAR_3,
  S_CHAR_4,
  S_CHAR_5,
  S_CHAR_6,
  S_CHAR_7,
  S_CHAR_8,
  S_CHAR_9,

  S_CHAR_A,
  S_CHAR_B,
  S_CHAR_C,
  S_CHAR_D,
  S_CHAR_E,
  S_CHAR_F,
  S_CHAR_G,
  S_CHAR_H,
  S_CHAR_I,
  S_CHAR_J,
  S_CHAR_K,
  S_CHAR_L,
  S_CHAR_M,
  S_CHAR_N,
  S_CHAR_O,
  S_CHAR_P,
  S_CHAR_Q,
  S_CHAR_R,
  S_CHAR_S,
  S_CHAR_T,
  S_CHAR_U,
  S_CHAR_V,
  S_CHAR_W,
  S_CHAR_X,
  S_CHAR_Y,
  S_CHAR_Z,

  S_MAX
};

extern std::vector<Mesh> g_mesh_list;
extern std::vector<LineVectorSprite> g_sprite_list;

bool load_assets();


