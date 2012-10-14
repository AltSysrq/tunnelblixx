#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "player.hxx"
#include "model.hxx"

#define HW 0.1f
#define HH 0.075f
#define FZ -0.3f
#define BZ 0.05f
#define GRAVITY -5.0f

static Model playerModel(
  true,
  M_COL,0.0,0.0,0.0,
  M_VER,0.0,0.0, BZ,
  M_COL,0.0,0.0, 0.2f,
  M_VER, HW,0.0,0.0,
  M_VER,0.0, HH,0.0,

  M_VER,0.0, HH,0.0,
  M_VER,-HW,0.0,0.0,
  M_COL,0.0,0.0,0.0,
  M_VER,0.0,0.0, BZ,

  M_VER,0.0,0.0, BZ,
  M_COL,0.0,0.0,0.2f,
  M_VER,-HW,0.0,0.0,
  M_VER,0.0,-HH,0.0,

  M_VER,0.0,-HH,0.0,
  M_VER, HW,0.0,0.0,
  M_COL,0.0,0.0,0.0,
  M_VER,0.0,0.0, BZ,

  M_COL,1.0,1.0,1.0,
  M_VER,0.0,0.0, FZ,
  M_COL,0.0,0.0, 0.2f,
  M_VER, HW,0.0,0.0,
  M_VER,0.0, HH,0.0,

  M_COL,1.0,1.0,1.0,
  M_VER,0.0,0.0, FZ,
  M_COL,0.0,0.0, 0.2f,
  M_VER,0.0, HH,0.0,
  M_VER,-HW,0.0,0.0,

  M_COL,1.0,1.0,1.0,
  M_VER,0.0,0.0, FZ,
  M_COL,0.0,0.0, 0.2f,
  M_VER,-HW,0.0,0.0,
  M_VER,0.0,-HH,0.0,

  M_COL,1.0,1.0,1.0,
  M_VER,0.0,0.0, FZ,
  M_COL,0.0,0.0, 0.2f,
  M_VER,0.0,-HH,0.0,
  M_VER, HW,0.0,0.0,

  M_END
);

Player::Player(GameField* field, const Distortion* dist,
               void (*notifier)(void*), void* notifyee)
: ModelledObject(field, 0.5f, playerModel.getH()/2, -playerModel.getL()/2-1,
                 playerModel, dist),
  vy(0), notifyOnDeath(notifier), notifyObject(notifyee)
{
}

void Player::kill() {
  notifyOnDeath(notifyObject);
  alive = false;
}

void Player::move(float x) {
  moveTo(x, y, z, true);
}

void Player::advance(float z, float offset) {
  moveTo(x, y, z-l/2-offset, true);
}

void Player::jump() {
  if (y - h*2/3 < 0)
    vy = 2.0f;
}

void Player::update(float et) {
  vy += GRAVITY*et;
  moveTo(x, y+vy*et, z, true);
}

void Player::collideWith(GameObject*) {}
