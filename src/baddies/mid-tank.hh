
#pragma

#include <vector>

#include "../math/vector3.hh"
#include "../mesh-instance.hh"
#include "../player-tank.hh"

// REVIEW: this is ported from ruby, so it may not make so
// much sense

class TankMetric {
private:

  float m_obs_distance;
  float m_obs_angle;
  float m_obs_side;
  Vector3 m_obs_vector;

  float m_tgt_distance;
  float m_tgt_angle;
  float m_tgt_side;

public:

  TankMetric( Vector3&, Vector3&, PlayerTank*, std::vector<MeshInstance>& );

  float obstacle_distance();
  float obstacle_angle();
  float obstacle_side();
  Vector3 & obstacle_vector();

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
  Vector3 m_mark_point;
  Vector3 m_obs_vector;
  float m_side;
  
public:

  TankObstacle();

  void set( Vector3&, TankMetric& );
  void clear();
  bool is_active();

  float distance_to( Vector3& );
  float angle_to( Vector3& );
  float side();
};

class MidTank {
private:

  Vector3 m_position;
  Vector3 m_direction;

  float m_heading;
  bool m_active;

  MeshInstance m_mesh_instance;
  
  TankObstacle m_obstacle;

  TankFireControl m_fire_control;
  
  bool shoot_at_target( TankMetric&, float&, Vector3& );
  bool sidestep_obstacle( TankMetric&, float&, Vector3& );
  bool turn_away_from_obstacle( Vector3&, TankMetric&, float&, Vector3& );
  bool turn_toward_target( TankMetric&, float&, Vector3& );
  bool move_toward_target( TankMetric&, float&, Vector3& );

public:

  MidTank();

  void set_pos( float, float );
  void think_and_move( PlayerTank*, std::vector<MeshInstance>& );
  bool is_active();
  void fire();

  MeshInstance & mesh_instance();
};

