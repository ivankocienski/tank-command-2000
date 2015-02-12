
#pragma once

#include "math/vector3.hh"
#include "window.hh"
#include "player.hh"

class Application {
private:

  static const int XRES;
  static const int YRES;
  static const float screen_ratio;

  static const float p_near;
  static const float p_far;
  static const float fov;

  Window m_window;

  Player m_player;

public:
  
  Application( int, char** );

  int main();
};

