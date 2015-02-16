
#include <iostream>

#include "window.hh"
#include "player.hh"
#include "camera.hh"
#include "world.hh"
#include "common.hh"
#include "application.hh"
#include "assets.hh"

using namespace std;

static const int XRES = 640;
static const int YRES = 480;

// near and far clip planes. just randomly picked
static const float P_NEAR = 0.25;
static const float P_FAR  = 20.0;
static const float FOV    = 90.0; // horizontal

Application::Application( int argc, char ** argv ) { 
}

int Application::main() {

  Window window;

  if( !window.open( XRES, YRES, "tc2k" )) {
    cerr << "could not open window" << endl;
    return -1;
  }

  if( !load_assets() ) {
    cerr << "could not load assets" << endl;
    return -1;
  }

  Camera camera(window);
  camera.setup( FOV, P_NEAR, P_FAR );

  Player player;

  World world;

  world.setup( window, camera, player );

  world.run();

  return 0;
}


