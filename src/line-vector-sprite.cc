
#include "line-vector-sprite.hh"

#include <iostream>

#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

using namespace std;

LineVectorSprite::LineVectorSprite() {
  m_width = 0;
  m_height = 0;
}

bool LineVectorSprite::load( const char* filepath ) {

  bool okay = false;
  FILE *file;
  
  char sig[4] = { 0, 0, 0, 0 };
  int32_t line_count;
  float *line_buffer = NULL;
  int32_t w;
  int32_t h;

  for(;;) {

    file = fopen( filepath, "r" );
    if(!file) break;

    fread( sig, sizeof(char), 3, file );
    if( strcmp( sig, "LV1" )) break; 

    fread( &line_count, sizeof(int32_t), 1, file );
    fread( &w, sizeof(int32_t), 1, file );
    fread( &h, sizeof(int32_t), 1, file );

    line_buffer = (float*)calloc( line_count, sizeof(T_LINE));
    if(!line_buffer) break;

    fread( line_buffer, line_count, sizeof(T_LINE), file );

    m_lines.resize(line_count);

    float *lp = line_buffer;
    for( int i = 0; i < line_count; i++ ) {
      m_lines[i].set( lp[0], lp[1], lp[2], lp[3] );
      lp += 4;
    }

    okay = true;

    break;
  }

  cout << "line vector sprite load " << (okay ? "okay" : "fail") << endl;
  cout << "  lines.size=" << m_lines.size() << endl;

  if(file) fclose(file);
  if(line_buffer) free( line_buffer ); 

  m_width  = w;
  m_height = h;

  return true;
}

void LineVectorSprite::draw( Window & w, int x, int y ) {
  
  vector<T_LINE>::iterator it;

  if( x < -m_width )   return;
  if( y < -m_height )  return;
  if( x > w.width() )  return;
  if( y > w.height() ) return;
  

  for( it = m_lines.begin(); it != m_lines.end(); it++ ) {

    w.draw_line(
        x + it->x1,
        y + it->y1,
        x + it->x2,
        y + it->y2,
        255
        );
  }
}
