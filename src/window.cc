
#include <cstring>
#include <iostream>
using std::cerr;
using std::cout;
using std::endl;

#include "window.hh"
#include "line.hh"
#include <SDL/SDL_image.h>

Window::Window() {
  m_screen = NULL;
  m_active = false;
  memset( m_keys, false, sizeof(m_keys) );
}

Window::~Window() {
  SDL_Quit();
}

bool Window::open( int xr, int yr, const char *t ) {

  if(SDL_Init(SDL_INIT_VIDEO) != 0) {
    cerr << "Window::open could not init SDL" << endl;
    return false;
  }

  m_screen = SDL_SetVideoMode( xr, yr, 8, SDL_DOUBLEBUF | SDL_SWSURFACE );
  if(!m_screen) {
    cerr << "Window::open could not open screen" << endl;
    return false;
  }

  SDL_WM_SetCaption( t, 0 );
  
  SDL_Color *palbuff = NULL;
  FILE *file = fopen( "data/palette.bin", "r" );
  if( !file ) {
    cerr << "Window:: could not open palette file" << endl;
    return false;
  }

  char buff[4];
  fread( buff, 4, sizeof(char), file );
  
  if( strncmp( buff, "PAL8", 4 ) ) {
    cerr << "Window: palette not of correct type" << endl;
    return false;
  }

  unsigned char count;
  fread( &count, 1, sizeof(unsigned char), file );
  cout << "Window: palette has " << (int)count << " colors" << endl;

  unsigned char readpal[256 * 3];
  memset( readpal, 0xff, sizeof( readpal ));
  fread( readpal, sizeof(unsigned char), (int)count * 3, file );
  fclose(file);

  SDL_Color palette[256];
  memset( palette, 0, sizeof( palette ));

  unsigned char *rpp = readpal;
  for( int i = 0; i < count; i++ ) {

    palette[i].r = *rpp; rpp++;
    palette[i].g = *rpp; rpp++;
    palette[i].b = *rpp; rpp++;
  }

  SDL_SetPalette( m_screen, SDL_LOGPAL, palette, 0, 256 ); 

  m_active = true;
  SDL_LockSurface(m_screen);
  m_vbuff = (unsigned char *)m_screen->pixels;
  setup_line_drawer( m_vbuff, m_screen->w, m_screen->w-1, m_screen->h-1 );

  return true;
}

void Window::set_title( const char *t ) {
  SDL_WM_SetCaption( t, 0 );
}

int Window::width() {
  return m_screen->w;
}

int Window::height() {
  return m_screen->h;
}

float Window::aspect_ratio() {
  return (float)m_screen->w / (float)m_screen->h;
}

bool Window::active() {
  return m_active;
}

void Window::clear() {
  SDL_FillRect( m_screen, NULL, 0 );
}

void Window::tick() {

  SDL_Event event;

  SDL_UnlockSurface(m_screen); 
  SDL_Flip(m_screen);
  SDL_FillRect( m_screen, NULL, 0 );
  SDL_Delay(20);

  SDL_LockSurface(m_screen);
  m_vbuff = (unsigned char *)m_screen->pixels;

  while(SDL_PollEvent( &event )) {
    switch( event.type ) {
      case SDL_QUIT:
        m_active = false;
        break;

      case SDL_KEYUP:

        switch( event.key.keysym.sym ) {
          case SDLK_UP:    m_keys[K_UP]    = false; break;
          case SDLK_DOWN:  m_keys[K_DOWN]  = false; break;
          case SDLK_LEFT:  m_keys[K_LEFT]  = false; break;
          case SDLK_RIGHT: m_keys[K_RIGHT] = false; break;
          case SDLK_z:     m_keys[K_Z]     = false; break;
          case SDLK_x:     m_keys[K_X]     = false; break;
          case SDLK_c:     m_keys[K_C]     = false; break;
          case SDLK_s:     m_keys[K_S]     = false; break;
          case SDLK_q:     m_keys[K_Q]     = false; break;
          case SDLK_SPACE: m_keys[K_SPACE] = false; break;
          default: break;
        }
        break;

      case SDL_KEYDOWN:
        switch( event.key.keysym.sym ) {
          case SDLK_UP:    
            push_key(K_UP);
            m_keys[K_UP] = true; 
            break;

          case SDLK_DOWN:  
            push_key(K_DOWN);
            m_keys[K_DOWN] = true; 
            break;

          case SDLK_LEFT:  
            push_key(K_LEFT);
            m_keys[K_LEFT] = true;
            break;

          case SDLK_RIGHT:
            push_key(K_RIGHT);
            m_keys[K_RIGHT] = true;
            break;

          case SDLK_s:
            m_keys[K_S] = true;
            break;

          case SDLK_x:
            m_keys[K_X] = true;
            break;

          case SDLK_z:
            m_keys[K_Z] = true;
            break;

          case SDLK_c:
            m_keys[K_C] = true;
            break;

          case SDLK_q:
            m_keys[K_Q] = true;
            break;

          case SDLK_SPACE: 
            m_keys[K_SPACE] = true;
            push_key(K_SPACE); 
            break; 

          case SDLK_ESCAPE: push_key(K_ESCAPE); break; 
          case SDLK_RETURN: push_key(K_ENTER); break; 
          case SDLK_TAB: push_key(K_TAB); break;
          case SDLK_F1:  push_key(K_F1);  break;
          case SDLK_F12: push_key(K_F12); break;

          default: break;
        }
        break;

      case SDL_MOUSEMOTION:
        m_mouse_x = event.motion.x;
        m_mouse_y = event.motion.y;
        break;
    }
  }
}

int Window::mouse_x() {
  return m_mouse_x;
}

int Window::mouse_y() {
  return m_mouse_y;
}


void Window::push_key( int k ) {

  m_key_buffer.push_back(k);
  if( m_key_buffer.size() > MAX_KEY_HISTORY ) 
    m_key_buffer.pop_front();
}

void Window::flush_keys() {
  m_key_buffer.clear();

}

int Window::inkey() {

  if( m_key_buffer.empty() ) return K_NULL;

  int k = m_key_buffer.front();
  m_key_buffer.pop_front();

  return k; 
}

void Window::draw_line( int x1, int y1, int x2, int y2, unsigned char c ) {
  if(!m_vbuff) return;

  if( !clip_line( x1, y1, x2, y2, &x1, &y1, &x2, &y2 ) ) return;

  render_line( x1, y1, x2, y2, c );
}

void Window::draw_pixel( int x, int y, unsigned char c ) {
  if( x < 0 || x >= m_screen->w ) return;
  if( y < 0 || y >= m_screen->h ) return;

  m_vbuff[ y * m_screen->w + x ] = c;
}

void Window::draw_pixel2( int x, int y, unsigned char c ) {
  if( x < 0 || x + 2 > m_screen->w ) return;
  if( y < 0 || y + 2 > m_screen->h ) return;

  unsigned char *p = m_vbuff + y * m_screen->w + x;

  *p = c; p++;
  *p = c; p += m_screen->w - 1;
  *p = c; p++;
  *p = c;
}

