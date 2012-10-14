#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL_opengl.h>

#include "game.hxx"
#include "globals.hxx"

Game::Game() {}
Game::~Game() {}

void Game::update(float et) {
  tunnel.update(et);
}

void Game::configureGL() {
  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogf(GL_FOG_DENSITY, 0.25f);
  glFogf(GL_FOG_START, 1.0f);
  glFogf(GL_FOG_END, 16.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1, 1, -vheight, vheight, 1.0f, 128.0f);
  glScalef(2.0f, 2.0f, 1);
  glTranslatef(-0.5f, -vheight/2.0f, 0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Game::draw() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  tunnel.draw();
}

void Game::motion(float,float) {}
void Game::button(unsigned) {}
bool Game::running() const {
  return true;
}
