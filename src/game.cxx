#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "game.hxx"
#include "globals.hxx"

Game::Game() {}
Game::~Game() {}

void Game::update(float et) {}
void Game::configureGL() {}
void Game::draw() {}
void Game::motion(float,float) {}
void Game::button(unsigned) {}
bool Game::running() const {
  return true;
}
