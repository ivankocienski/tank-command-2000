
#pragma once

#include "math/vector3.hh"
#include "window.hh"
#include "player.hh"

class Application {
private:

  static const int XRES;
  static const int YRES;

  static const float P_NEAR;
  static const float P_FAR;
  static const float FOV;

  Window m_window;

  Player m_player;

public:
  
  Application( int, char** );

  int main();
};

