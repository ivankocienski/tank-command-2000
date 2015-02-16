
#include "assets.hh"

using namespace std;

vector<Mesh> g_mesh_list;

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

  return true;
}

