
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

  LOAD_MESH( A_OBSTACLE1, "obstacle-1.vex" );
  LOAD_MESH( A_OBSTACLE2, "obstacle-2.vex" );
  LOAD_MESH( A_OBSTACLE3, "obstacle-3.vex" );
  LOAD_MESH( A_OBSTACLE4, "obstacle-4.vex" );
  LOAD_MESH( A_MID_TANK, "tank.vex" );

  g_sprite_list.resize( S_MAX );

  LOAD_SPRITE( S_MINI_LOGO, "mini-logo.lvs" );
  LOAD_SPRITE( S_BG1, "bg1.lvs" );
  LOAD_SPRITE( S_BG2, "bg2.lvs" );
  LOAD_SPRITE( S_BG3, "bg3.lvs" );
  LOAD_SPRITE( S_BG4, "bg4.lvs" );

  return true;
}

