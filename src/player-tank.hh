
#pragma once

#include <vector>
#include <list>

#include "math/vector2.hh"
#include "math/matrix3.hh"
#include "mesh-instance.hh"
#include "obstacle.hh"

class Camera;
class World;

class PlayerTank {
private:

  typedef struct _S_CRACK {
    int sprite_id;
    Vector2 pos;
  } T_CRACK, *PT_CRACK;

  std::list<T_CRACK> m_cracks;

  World *m_world;

  Vector2 m_position;
  Vector2 m_direction;
  Vector2 m_right;

  Matrix3 m_inv_model;

  float m_want_walk;
  float m_want_turn;

  float m_walk_speed;
  float m_turn_speed;

  float m_heading;
  float m_height;

  int  m_hit_stun;
  float m_stun_speed;
  Vector2 m_stun_dir;
  
  int  m_armour;
  bool m_fire_now;
  
  void do_move( std::vector<Obstacle> & );
  
  void adjust_cracks();
  
public:

  PlayerTank(World*);
  void reset();

  int armour();
  void do_damage(int);
  void add_armour(int);
  
  Vector2& position();
  void set_pos( float, float );

  void look(Camera*);
  float heading();

  bool is_touching( const Vector2& );
  
  void move( std::vector<Obstacle> & );
  void fire(bool);

  void want_walk(float);
  void want_turn(float);

  void draw_cracks(Window*);
  
  Matrix3 & inv_model_matrix();
};

