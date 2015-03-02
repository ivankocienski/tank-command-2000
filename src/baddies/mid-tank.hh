
#pragma once

#include <vector>

#include "../math/vector2.hh"
#include "../mesh-instance.hh"
#include "../player-tank.hh"
#include "../obstacle.hh"

class World;

// REVIEW: this is ported from ruby, so it may not make so
// much sense

class TankMetric {
private:

  float m_obs_distance;
  float m_obs_angle;
  float m_obs_side;
  Vector2 m_obs_vector;

  float m_tgt_distance;
  float m_tgt_angle;
  float m_tgt_side;

public:

  TankMetric( Vector2&, Vector2&, PlayerTank*, std::vector<Obstacle>& );

  float obstacle_distance();
  float obstacle_angle();
  float obstacle_side();
  Vector2 & obstacle_vector();

  float target_angle();
  float target_distance();
  float target_side();
};

class TankFireControl {
private:

  typedef struct _S_FIRE_TABLE {
    int   pos;
    float distance;
    int   hold;
  } T_FIRE_TABLE, *PT_FIRE_TABLE;

  static T_FIRE_TABLE s_fire_table[];

  PT_FIRE_TABLE m_fire_entry;
  int  m_fire_hold;
  bool m_is_locked_on;
  bool m_should_fire;
  
public:

  TankFireControl();

  void scan( TankMetric& );

  bool is_locked_on();
  bool should_fire();
};

class TankObstacle {
private:

  bool m_active;
  Vector2 m_mark_point;
  Vector2 m_obs_vector;
  float m_side;
  
public:

  TankObstacle();

  void set( Vector2&, TankMetric& );
  void clear();
  bool is_active();

  float distance_to( Vector2& );
  float angle_to( Vector2& );
  float side();
};

class MidTank {
private:

  World* m_world;

  Vector2 m_start_pos;
  Vector2 m_position;
  Vector2 m_direction;

  float m_heading;
  float m_height;
  bool m_active;

  MeshInstance m_mesh_instance;
  
  TankObstacle m_obstacle;

  TankFireControl m_fire_control;
  
  bool shoot_at_target( TankMetric&, float&, Vector2& );
  bool sidestep_obstacle( TankMetric&, float&, Vector2& );
  bool turn_away_from_obstacle( Vector2&, TankMetric&, float&, Vector2& );
  bool turn_toward_target( TankMetric&, float&, Vector2& );
  bool move_toward_target( TankMetric&, float&, Vector2& );

public:

  MidTank();
  MidTank(World*);

  void set_pos( float, float );
  void reset();
  
  void think_and_move( PlayerTank*, std::vector<Obstacle>& );
  bool is_active();
  void fire();
  void deactivate();

  MeshInstance & mesh_instance();
};

