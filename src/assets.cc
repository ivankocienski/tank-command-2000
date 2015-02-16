
#include "assets.hh"

using namespace std;

vector<Mesh> g_mesh_list;
vector<LineVectorSprite> g_sprite_list;

bool load_assets() {

  bool l;

  g_mesh_list.resize(A_MAX);

  l = g_mesh_list[A_TANK].load_from( "data/tank.vex" );
  if( !l ) return false;

  l = g_mesh_list[A_OBSTACLE1].load_from( "data/obstacle-1.vex" );
  if( !l ) return false;

  l = g_mesh_list[A_OBSTACLE2].load_from( "data/obstacle-2.vex" );
  if( !l ) return false;

  l = g_mesh_list[A_OBSTACLE3].load_from( "data/obstacle-3.vex" );
  if( !l ) return false;

  l = g_mesh_list[A_OBSTACLE4].load_from( "data/obstacle-4.vex" );
  if( !l ) return false;

  g_sprite_list.resize( S_MAX );

  l = g_sprite_list[S_MINI_LOGO].load( "data/mini-logo.lvs" );
  if( !l ) return false;

  l = g_sprite_list[S_BG1].load( "data/bg1.lvs" );
  if( !l ) return false;

  l = g_sprite_list[S_BG2].load( "data/bg2.lvs" );
  if( !l ) return false;

  l = g_sprite_list[S_BG3].load( "data/bg3.lvs" );
  if( !l ) return false;

  l = g_sprite_list[S_BG4].load( "data/bg4.lvs" );
  if( !l ) return false;

  return true;
}

