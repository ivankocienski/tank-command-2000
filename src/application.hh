
#pragma once

#include "window.hh"
#include "camera.hh"

class Application {
private:

  enum { // menu outcome
    MO_PLAY,
    MO_HELP,
    MO_QUIT
  };

  Window m_window;
 
  int  do_menu( Camera& );
  void do_help();
  
public:
  
  Application( int, char** );

  void draw_text( int, int, const char* );
  void draw_hud_number(int, int, unsigned int );

  int main();
};

