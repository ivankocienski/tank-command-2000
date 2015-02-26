
#pragma once

#include "player-tank.hh"

class World;

class Player {
private:

  //World *m_world;
  
  int m_score;
  int m_lives;

  PlayerTank m_tank;

public:

  Player(World*);

  void reset();

  PlayerTank & current_tank();

  int tank_count();
  int current_score();

};
