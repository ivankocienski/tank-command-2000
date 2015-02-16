
#pragma once

#include "player-tank.hh"

class Player {
private:

  int m_score;
  int m_lives;

  PlayerTank m_tank;

public:

  Player();

  void reset();

  PlayerTank & current_tank();

};
