#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <SDL_opengl.h>
#include <SDL.h>

#include <algorithm>
#include <cmath>

#include "game.hxx"
#include "player.hxx"
#include "globals.hxx"
#include "music_player.hxx"

#define ACCEL -0.015f
#define MIN_CONV 0.5f
#define MAX_CONV 2.5f

using namespace std;

Game::Game()
: currentConvulsion(0), targetConvulsion(0),
  playerAmplitude(0),
  enemyFactory(*this),
  speed(-2.0f)
{
  player = new Player(&field, &distortion, getNearClippingPlane(),
                      &playerDeath, this);
  field.add(player);
}

Game::~Game() {}

void Game::update(float et) {
  float distortionDiff = targetConvulsion - currentConvulsion;
  distortionDiff *= pow(0.2f, et);
  currentConvulsion = targetConvulsion - distortionDiff;
  distortion.setConvulsionMult(currentConvulsion);

  if (player)
    player->setAmplitude(playerAmplitude);

  enemyFactory.update(et, -speed);
  tunnel.update(et);
  field.update(et);

  float oldNear = getNearClippingPlane();
  speed += ACCEL*et;
  float nearDelta = getNearClippingPlane() - oldNear;
  float shift = speed*et;
  if (player) {
    player->shotSpeed = -speed;

    float oldz = player->getZ();
    player->advance(shift, nearDelta);
    if (player)
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

static void amplitudeCallback(void* thisVoid, float amp) {
  ((Game*)thisVoid)->amplitude(amp);
}

void Game::amplitude(float amp) {
  // XXX Minor threading issues here; we could see a partial floating-point
  // update. Oh well.
  targetConvulsion = amp*(MAX_CONV - MIN_CONV) + MIN_CONV;
  playerAmplitude = amp;
}

void Game::startMusic(const char*const* list, unsigned len) {
  mixer.play(new MusicPlayer(list, len, amplitudeCallback, this), 0x7FFF);
}
