
#pragma once

#include "player-tank.hh"

class World;

class Player {
private:

  //World *m_world;
  
  int m_score;
  int m_lives;
  int m_kills;
  int m_kill_threshold;
  
  int m_difficulty;

  PlayerTank m_tank;

public:

  Player(World*);

  void reset();

  PlayerTank & current_tank();

  int tank_count();
  void take_tank();
  
  int current_score();
  void add_score(int);

  void add_kill(); 
  int difficulty();
};
