
#pragma once

#include <vector>
#include "window.hh"

class LineVectorSprite {
private:

  int m_width;
  int m_height;
  
  typedef struct _S_LINE {
    float x1;
    float y1;
    float x2;
    float y2;

    void set( float _x1, float _y1, float _x2, float _y2 ) { x1 = _x1; y1 = _y1; x2 = _x2; y2 = _y2; }
  } T_LINE, *PT_LINE;

  std::vector<T_LINE> m_lines;

public:
  
  LineVectorSprite();
  
  bool load( const char* );

  void draw( Window&, int, int );
  void draw( Window&, int, int, int );
};

