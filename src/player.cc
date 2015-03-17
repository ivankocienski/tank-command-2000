
#include "world.hh"
#include "player.hh"

Player::Player(World* w) : m_tank(w) {
  //m_world = w;
  reset();
}

void Player::reset() {
  m_score = 0;
  m_lives = 5;
  m_kills = 0;
  m_kill_threshold = 3;
  m_difficulty = 0;
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

void Player::add_kill() {
  m_kills++;
  if(m_kills >= m_kill_threshold ) {
    m_difficulty++;
    m_kill_threshold += 3;
  }
}

int Player::difficulty() {
  return m_kills;
}
