
#include "world.hh"
#include "player.hh"

Player::Player(World* w) : m_tank(w) {
  //m_world = w;
  reset();
}

void Player::reset() {
  m_score = 0;
  m_lives = 5;
}

PlayerTank & Player::current_tank() {
  return m_tank;
}

void Player::add_score( int d ) {
  m_score += d;
}

int Player::tank_count() {
  return m_lives;
}

void Player::take_tank() {
  m_lives--;
  m_tank.reset();
}

int Player::current_score() {
  return m_score;
}
