
#include <iostream>
using namespace std;

#include <math.h>

enum {
  C_LEFT  = 8, // 1000
  C_RIGHT = 4, // 0100
  C_UP    = 2, // 0010
  C_DOWN  = 1  // 0001
};

static unsigned char* vbuff;
static int vpitch;
static int max_x;
static int max_y;

void setup_line_drawer( unsigned char* vb, int vp, int max, int may ) {
  vbuff  = vb;
  vpitch = vp;
  max_x  = max;
  max_y  = may;
}

void render_line( int x1, int y1, int x2, int y2, unsigned char c ) {

  int dx, dy;
  int sx, sy;
  int err, err2;

  dx = fabs(x2 - x1);
  dy = fabs(y2 - y1); 

  sx = (x1 < x2) ? 1 : -1;
  sy = (y1 < y2) ? 1 : -1;

  err = dx - dy;

  while(1) {

    vbuff[ (y1 * vpitch) + x1 ] = c;

    if( x1 == x2 && y1 == y2) break;

    err2 = err << 1;

    if(err2 > -dy) { err -= dy; x1  += sx; }
    if(err2 <  dx) { err += dx; y1  += sy; }
  } 
}

inline float dist( float x1, float y1, float x2, float y2 ) {
  float dx = x2 - x1;
  float dy = y2 - y1;

  return sqrt( dx * dx + dy * dy ); 
}

bool clip_line( int x1, int y1, int x2, int y2, int *ox1, int *oy1, int *ox2, int *oy2 ) {

  char clip_a = 0;
  char clip_b = 0;

  if( x1 < 0 ) clip_a |= C_LEFT;
  if( x2 < 0 ) clip_b |= C_LEFT;

  if( y1 < 0 ) clip_a |= C_UP;
  if( y2 < 0 ) clip_b |= C_UP;

  if( x1 >= max_x ) clip_a |= C_RIGHT;
  if( x2 >= max_x ) clip_b |= C_RIGHT;

  if( y1 >= max_y ) clip_a |= C_DOWN;
  if( y2 >= max_y ) clip_b |= C_DOWN;

  if( (clip_a & clip_b) & C_LEFT  ) return false;
  if( (clip_a & clip_b) & C_DOWN  ) return false;
  if( (clip_a & clip_b) & C_UP    ) return false;
  if( (clip_a & clip_b) & C_RIGHT ) return false;

  float dx = (float)(x2 - x1);
  float dy = (float)(y2 - y1);
  float s1 = dy / dx; 
  float s2 = dx / dy; 
  int nx1, ny1;
  int nx2, ny2;
  int d1;
  int d2;
  
  switch( clip_a ) {
    case C_LEFT | C_UP: //okay
      
      //up
      d1 = 0 - y1;
      nx1 = x1 + s2 * d1;
      ny1 = 0;

      //left
      d2 = 0 - x1;
      nx2 = 0;
      ny2 = y1 + s1 * d2;

      if( dist(nx1, ny1, x2, y2) < dist(nx2, ny2, x2, y2) ) {
        x1 = nx1;
        y1 = ny1;
      } else {
        x1 = nx2;
        y1 = ny2;
      }
      break; 

    case C_LEFT: // okay
      d1 = 0 - x1;
      y1 += s1 * d1;
      x1 = 0;
      break;

    case C_LEFT | C_DOWN: // okay

      // dpwn
      d1 = max_y - y1;
      nx1 = x1 + s2 * d1;
      ny1 = max_y - 1;

      // left
      d2 = 0 - x1;
      nx2 = 0;
      ny2 = y1 + s1 * d2;

      if( dist(nx1, ny1, x2, y2) < dist(nx2, ny2, x2, y2) ) {
        x1 = nx1;
        y1 = ny1;
      } else {
        x1 = nx2;
        y1 = ny2;
      }

      break;

    case C_UP: // okay
      d1 = 0 - y1;
      x1 += s2 * d1;
      y1 = 0; 
      break;

    case C_DOWN: // okay
      d1 = max_y - y1;
      x1 += s2 * d1;
      y1 = max_y - 1;
      break;

    case C_RIGHT | C_UP:

      // up
      d1  = 0 - y1;
      nx1 = x1 + s2 * d1;
      ny1 = 0; 

      // right
      d2  = x1 - max_x;
      nx2 = max_x - 1;
      ny2 = y1 - s1 * d2;

      if( dist(nx1, ny1, x2, y2) < dist(nx2, ny2, x2, y2) ) {
        x1 = nx1;
        y1 = ny1;
      } else {
        x1 = nx2;
        y1 = ny2;
      }

      break;

    case C_RIGHT:
      d1  = x1 - max_x;
      y1 -= s1 * d1;
      x1  = max_x - 1;
      break;

    case C_RIGHT | C_DOWN:

      //right
      d1  = x1 - max_x;
      nx2 = max_x - 1;
      ny2 = y1 - s1 * d1;

      //down
      d2 = max_y - y1;
      nx1 = x1 + s2 * d2;
      ny1 = max_y - 1;

      if( dist(nx1, ny1, x2, y2) < dist(nx2, ny2, x2, y2) ) {
        x1 = nx1;
        y1 = ny1;
      } else {
        x1 = nx2;
        y1 = ny2;
      }
      break;
  }

  // have we clipped ourselves to invisibiltiy?
 
    if( clip_b & C_UP ) {
      if( y1 < 0 ) return false;
    } else
      if( clip_b & C_DOWN ) {
	if( y1 >= max_y ) return false;
      }

    if( clip_b & C_LEFT ) {
      if( x1 < 0 ) return false;
    } else
      if( clip_b & C_RIGHT ) {
	if( x1 >= max_x ) return false;
      }
    

  switch( clip_b ) {
    case C_LEFT | C_UP: //okay
      
      //up
      d1 = 0 - y2;
      nx1 = x2 + s2 * d1;
      ny1 = 0;

      //left
      d2 = 0 - x2;
      nx2 = 0;
      ny2 = y2 + s1 * d2;

      if( dist(nx1, ny1, x1, y1) < dist(nx2, ny2, x1, y1) ) {
        x2 = nx1;
        y2 = ny1;
      } else {
        x2 = nx2;
        y2 = ny2;
      }
      break; 

    case C_LEFT: // okay
      d1 = 0 - x2;
      y2 += s1 * d1;
      x2 = 0;
      break;

    case C_LEFT | C_DOWN: // okay

      // dpwn
      d1 = max_y - y2;
      nx1 = x2 + s2 * d1;
      ny1 = max_y - 1;

      // left
      d2 = 0 - x2;
      nx2 = 0;
      ny2 = y2 + s1 * d2;

      if( dist(nx1, ny1, x1, y1) < dist(nx2, ny2, x1, y1) ) {
        x2 = nx1;
        y2 = ny1;
      } else {
        x2 = nx2;
        y2 = ny2;
      }

      break;

    case C_UP: // okay
      d1 = 0 - y2;
      x2 += s2 * d1;
      y2 = 0; 
      break;

    case C_DOWN: // okay
      d1 = max_y - y2;
      x2 += s2 * d1;
      y2 = max_y - 1;
      break;

    case C_RIGHT | C_UP:

      // up
      d1  = 0 - y2;
      nx1 = x2 + s2 * d1;
      ny1 = 0; 

      // right
      d2  = x2 - max_x;
      nx2 = max_x - 1;
      ny2 = y2 - s1 * d2;

      if( dist(nx1, ny1, x1, y1) < dist(nx2, ny2, x1, y1) ) {
        x2 = nx1;
        y2 = ny1;
      } else {
        x2 = nx2;
        y2 = ny2;
      }

      break;

    case C_RIGHT:
      d1  = x2 - max_x;
      y2 -= s1 * d1;
      x2  = max_x - 1;
      break;

    case C_RIGHT | C_DOWN:

      //right
      d1  = x2 - max_x;
      nx2 = max_x - 1;
      ny2 = y2 - s1 * d1;

      //down
      d2 = max_y - y2;
      nx1 = x2 + s2 * d2;
      ny1 = max_y - 1;

      if( dist(nx1, ny1, x1, y1) < dist(nx2, ny2, x1, y1) ) {
        x2 = nx1;
        y2 = ny1;
      } else {
        x2 = nx2;
        y2 = ny2;
      }
      break;

  }

  if( clip_a & C_UP ) {
    if( y2 < 0 ) return false;
  } else
    if( clip_a & C_DOWN ) {
      if( y2 >= max_y ) return false;
    }

  if( clip_a & C_LEFT ) {
    if( x2 < 0 ) return false;
  } else
    if( clip_a & C_RIGHT ) {
      if( x2 >= max_x ) return false;
    }

  *ox1 = x1;
  *oy1 = y1;
  *ox2 = x2;
  *oy2 = y2;

  return true;
}

