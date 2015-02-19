
#pragma once

class Window;

class EnemyBase {
private:

  Window *m_window;

public:
  
  EnemyBase( Window* );

  virtual void move();
  virtual void draw();

  virtual bool active();
};
