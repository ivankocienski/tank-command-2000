
#pragma once

#include "window.hh"

class Application {
private:

  Window m_window;

public:
  
  Application( int, char** );

  void draw_text( int, int, const char* );
  int main();
};

