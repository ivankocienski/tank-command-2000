
#pragma once

#include <SDL/SDL.h>
#include <vector>
#include <list>

enum {
  C_RED       = 15,
  C_ORANGE    = 31,
  C_YELLOW    = 47,
  C_GREEN     = 63,
  C_TURQUOISE = 79,
  C_BLUE      = 95,
  C_PURPLE    = 111,
  C_PINK      = 127,
  C_WHITE     = 143
};

class Window {
public:

  enum {
    K_NULL,
    K_UP,
    K_DOWN,
    K_RIGHT,
    K_LEFT,
    K_ESCAPE,
    K_SPACE,
    K_ENTER,
    K_TAB,
    K_Z,
    K_S,
    K_X,
    K_C,
    K_Q,
    K_D,
    K_F1,
    K_F2,
    K_F3,
    K_F4,
    K_F5,
    K_F6,
    K_F7,
    K_F8,
    K_F9,
    K_F10,
    K_F11,
    K_F12,
    K_MAX
  };

private:

  static const int MAX_KEY_HISTORY = 32;

  SDL_Surface *m_screen;

  unsigned char *m_vbuff;
  bool m_active;

  int m_mouse_x;
  int m_mouse_y;

  void push_key(int);

public:

  std::list<int>m_key_buffer;
  
  bool m_keys[K_MAX];

  Window();
  ~Window();

  bool open( int, int, const char *, bool );
  void set_title( const char * );
  
  void clear();
  int width();
  int height();
  float aspect_ratio();

  bool active();
  void tick(); 

  int mouse_x();
  int mouse_y();

  void flush_keys();
  int inkey();

  void draw_line( int, int, int, int, unsigned char );
  void draw_pixel( int, int, unsigned char );
  void draw_pixel2( int, int, unsigned char );
};

