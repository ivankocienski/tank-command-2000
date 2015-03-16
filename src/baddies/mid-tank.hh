
#pragma once

#include <vector>

#include "../math/vector2.hh"
#include "../mesh-instance.hh"
#include "../player-tank.hh"
#include "../obstacle.hh"

class MidTank;
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

  TankMetric( MidTank*, PlayerTank*, std::vector<Obstacle>& );

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

  Vector2 m_mark_point;
  Vector2 m_obs_vector;
  float m_side;
  int m_mode;
  
public:

  enum {
    OM_INACTIVE,
    OM_TURN,
    OM_MOVE,
    OM_MAX
  };
  
  TankObstacle();

  void set( Vector2&, TankMetric& );
  void clear();
  bool is_active();
  int mode();
  void next_mode();

  float distance_to( Vector2& );
  float angle_to( Vector2& );
  float side();
};

class MidTank {
private:

  World* m_world;

  Vector2 m_position;
  Vector2 m_direction;

  float m_heading;
  float m_height;
  int   m_hit_points;
  bool  m_active;

  MeshInstance m_mesh_instance;
  
  TankObstacle m_obstacle;

  TankFireControl m_fire_control;

  void fire();
  
  bool shoot_at_target( TankMetric&, float&, Vector2& );
  bool sidestep_obstacle( TankMetric&, float&, Vector2& );
  bool turn_away_from_obstacle( Vector2&, TankMetric&, float&, Vector2& );
  bool detect_obstacle( TankMetric& );
  bool turn_toward_target( TankMetric&, float&, Vector2& );
  bool move_toward_target( TankMetric&, float&, Vector2& );

public:

  MidTank();
  MidTank(World*);

  void think_and_move( PlayerTank*, std::vector<Obstacle>& );

  void activate( const Vector2& );
  void give_damage( int );
  bool is_active();
  void deactivate();

  MeshInstance & mesh_instance();

  const Vector2 & position() const;
  const Vector2 & direction() const;
};

