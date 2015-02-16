
#include "player.hh"

Player::Player() {
  reset();
}

void Player::reset() {
  m_score = 0;
  m_lives = 5;
}

PlayerTank & Player::current_tank() {
  return m_tank;
}
