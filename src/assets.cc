
#include "assets.hh"

using namespace std;

vector<Mesh> g_mesh_list;
vector<LineVectorSprite> g_sprite_list;

#define LOAD_MESH( to, from ) \
  l = g_mesh_list[(to)].load_from( "data/" from ); \
  if( !l ) return false;

#define LOAD_SPRITE( to, from ) \
  l = g_sprite_list[(to)].load( "data/" from ); \
  if( !l ) return false;

bool load_assets() {

  bool l;

  g_mesh_list.resize(A_MAX);

  LOAD_MESH( A_OBSTACLE1,  "obstacle-1.vex" );
  LOAD_MESH( A_OBSTACLE2,  "obstacle-2.vex" );
  LOAD_MESH( A_OBSTACLE3,  "obstacle-3.vex" );
  LOAD_MESH( A_OBSTACLE4,  "obstacle-4.vex" );
  LOAD_MESH( A_OBSTACLE5,  "obstacle-5.vex" );
  LOAD_MESH( A_OBSTACLE6,  "obstacle-6.vex" );
  LOAD_MESH( A_OBSTACLE7,  "obstacle-7.vex" );
  LOAD_MESH( A_OBSTACLE8,  "obstacle-8.vex" );
  LOAD_MESH( A_OBSTACLE9,  "obstacle-9.vex" );
  LOAD_MESH( A_OBSTACLE10, "obstacle-10.vex" );

  LOAD_MESH( A_MID_TANK, "tank.vex" );
  LOAD_MESH( A_BULLET, "bullet.vex" );

  g_sprite_list.resize( S_MAX );

  LOAD_SPRITE( S_MINI_LOGO, "mini-logo.lvs" );
  LOAD_SPRITE( S_BG1, "bg1.lvs" );
  LOAD_SPRITE( S_BG2, "bg2.lvs" );
  LOAD_SPRITE( S_BG3, "bg3.lvs" );
  LOAD_SPRITE( S_BG4, "bg4.lvs" );

  LOAD_SPRITE( S_SPLASH_LOGO, "main-logo-text.lvs" );
  LOAD_SPRITE( S_CANNON_RETICULE, "cannon-reticule.lvs" );
  LOAD_SPRITE( S_HUD_BG, "hud-bg.lvs" ); 
  LOAD_SPRITE( S_SCREEN_CRACK, "screen-crash.lvs" );

  LOAD_SPRITE( S_HUD_NUM_0, "hud-num-0.lvs" );
  LOAD_SPRITE( S_HUD_NUM_1, "hud-num-1.lvs" );
  LOAD_SPRITE( S_HUD_NUM_2, "hud-num-2.lvs" );
  LOAD_SPRITE( S_HUD_NUM_3, "hud-num-3.lvs" );
  LOAD_SPRITE( S_HUD_NUM_4, "hud-num-4.lvs" );
  LOAD_SPRITE( S_HUD_NUM_5, "hud-num-5.lvs" );
  LOAD_SPRITE( S_HUD_NUM_6, "hud-num-6.lvs" );
  LOAD_SPRITE( S_HUD_NUM_7, "hud-num-7.lvs" );
  LOAD_SPRITE( S_HUD_NUM_8, "hud-num-8.lvs" );
  LOAD_SPRITE( S_HUD_NUM_9, "hud-num-9.lvs" );

  LOAD_SPRITE( S_CHAR_0, "char-0.lvs" );
  LOAD_SPRITE( S_CHAR_1, "char-1.lvs" );
  LOAD_SPRITE( S_CHAR_2, "char-2.lvs" );
  LOAD_SPRITE( S_CHAR_3, "char-3.lvs" );
  LOAD_SPRITE( S_CHAR_4, "char-4.lvs" );
  LOAD_SPRITE( S_CHAR_5, "char-5.lvs" );
  LOAD_SPRITE( S_CHAR_6, "char-6.lvs" );
  LOAD_SPRITE( S_CHAR_7, "char-7.lvs" );
  LOAD_SPRITE( S_CHAR_8, "char-8.lvs" );
  LOAD_SPRITE( S_CHAR_9, "char-9.lvs" );

  LOAD_SPRITE( S_CHAR_A, "char-a.lvs" );
  LOAD_SPRITE( S_CHAR_B, "char-b.lvs" );
  LOAD_SPRITE( S_CHAR_C, "char-c.lvs" );
  LOAD_SPRITE( S_CHAR_D, "char-d.lvs" );
  LOAD_SPRITE( S_CHAR_E, "char-e.lvs" );
  LOAD_SPRITE( S_CHAR_F, "char-f.lvs" );
  LOAD_SPRITE( S_CHAR_G, "char-g.lvs" );
  LOAD_SPRITE( S_CHAR_H, "char-h.lvs" );
  LOAD_SPRITE( S_CHAR_I, "char-i.lvs" );
  LOAD_SPRITE( S_CHAR_J, "char-j.lvs" );
  LOAD_SPRITE( S_CHAR_K, "char-k.lvs" );
  LOAD_SPRITE( S_CHAR_L, "char-l.lvs" );
  LOAD_SPRITE( S_CHAR_M, "char-m.lvs" );
  LOAD_SPRITE( S_CHAR_N, "char-n.lvs" );
  LOAD_SPRITE( S_CHAR_O, "char-o.lvs" );
  LOAD_SPRITE( S_CHAR_P, "char-p.lvs" );
  LOAD_SPRITE( S_CHAR_Q, "char-q.lvs" );
  LOAD_SPRITE( S_CHAR_R, "char-r.lvs" );
  LOAD_SPRITE( S_CHAR_S, "char-s.lvs" );
  LOAD_SPRITE( S_CHAR_T, "char-t.lvs" );
  LOAD_SPRITE( S_CHAR_U, "char-u.lvs" );
  LOAD_SPRITE( S_CHAR_V, "char-v.lvs" );
  LOAD_SPRITE( S_CHAR_W, "char-w.lvs" );
  LOAD_SPRITE( S_CHAR_X, "char-x.lvs" );
  LOAD_SPRITE( S_CHAR_Y, "char-y.lvs" );
  LOAD_SPRITE( S_CHAR_Z, "char-z.lvs" );

  return true;
}

