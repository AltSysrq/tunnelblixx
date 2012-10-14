#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL_opengl.h>
#include <SDL.h>

#include <algorithm>

#include "game.hxx"
#include "player.hxx"
#include "globals.hxx"

#define ACCEL -0.15f

using namespace std;

Game::Game()
: speed(-2.0f)
{
  player = new Player(&field, &distortion, getNearClippingPlane(),
                      &playerDeath, this);
  field.add(player);
}

Game::~Game() {}

void Game::update(float et) {
  tunnel.update(et);
  field.update(et);

  float oldNear = getNearClippingPlane();
  speed += ACCEL*et;
  float nearDelta = getNearClippingPlane() - oldNear;
  float shift = speed*et;
  if (player) {
    float oldz = player->getZ();
    player->advance(shift, nearDelta);
    shift = player->getZ() - oldz + nearDelta;
  }

  field.translateZ(shift);
  tunnel.translateZ(shift);
  distortion.translateZ(shift, -speed);
}

void Game::configureGL() {
  glEnable(GL_FOG);
  glFogi(GL_FOG_MODE, GL_LINEAR);
  glFogf(GL_FOG_DENSITY, 0.25f);
  glFogf(GL_FOG_START, max(1.0f, getNearClippingPlane()));
  glFogf(GL_FOG_END, max(1.1f, 1.1f*getSpawnDistance()));

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glFrustum(-1, 1, -vheight, vheight, getNearClippingPlane(), 128.0f);
  glScalef(2.0f, 2.0f, 1);
  glTranslatef(-0.5f, -vheight/2.0f, 0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

void Game::draw() {
  glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
  tunnel.draw(distortion);
  field.draw();
}

void Game::motion(float x,float) {
  if (player)
    player->move(x);
}

void Game::button(unsigned button) {
  if (player) {
    if (button == SDL_BUTTON_LEFT)
      player->jump();
  }
}

bool Game::running() const {
  return !!player;
}

void Game::playerDeath(void* that) {
  ((Game*)that)->player = NULL;
}

float Game::getNearClippingPlane() const {
  return -1.5f/speed;
}

float Game::getSpawnDistance() const {
  return min(8*-speed, Tunnel::gridLength*Tunnel::gsqlen/2);
}

float Game::getPlayerX() const {
  return player? player->getX() : 0.5f;
}
