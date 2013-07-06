#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>

#include "model.hxx"
#include "beetle.hxx"

#define DIM 0.1f
#define SPEED 0.5f

static const float modelData[] = {
  M_COL, 0, 0, 1,

  M_VER, -DIM, 0, -DIM,
  M_VER, +DIM, 0, -DIM,
  M_COL, 0, 0.5f, 1,
  M_VER, 0, 1.5f*DIM, 0,

  M_VER, 0, 1.5f*DIM, 0,
  M_COL, 0, 0, 1,
  M_VER, -DIM, 0, -DIM,
  M_VER, -DIM, 0, +DIM,

  M_VER, -DIM, 0, +DIM,
  M_VER, +DIM, 0, +DIM,
  M_COL, 0, 0.5f, 1,
  M_VER, 0, 1.5f*DIM, 0,

  M_VER, 0, 1.5f*DIM, 0,
  M_COL, 0, 0, 1,
  M_VER, +DIM, 0, -DIM,
  M_VER, +DIM, 0, +DIM,

  M_END, 0, 0, 0
};

static const Model beetleModel(modelData, true);

Beetle::Beetle(Game& game)
: Enemy(game, beetleModel, 2),
  vx((rand() & 1)? +SPEED : -SPEED)
{ }

void Beetle::update(float et) {
  bool moved = moveTo(x + vx*et, y, z, false);
  if (x-w/2 == 0 && vx < 0) moved = false;
  if (x+w/2 == 1 && vx > 0) moved = false;
  if (!moved)
    vx = -vx;
}

const float* Beetle::getColour() const {
  static const float colour[3] = { 0, 0, 1 };
  return colour;
}

float Beetle::getPulseSpeed() const { return 18; }
unsigned Beetle::getAward() const { return 500; }
const char* Beetle::getDeathSound() const { return "sound/beetle_death.pcm"; }
