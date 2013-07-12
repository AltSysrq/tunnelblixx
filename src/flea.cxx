#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>
#include <cstdlib>

#include "flea.hxx"
#include "model.hxx"
#include "game.hxx"

using namespace std;

#define HW 0.15f
#define H 0.275f
#define LH 0.1f
#define L 0.25f
#define GRAVITY -4.5f

static const float fleaModelData[] = {
  M_COL, 1, 1, 0,
  M_VER, -HW, 0, 0,
  M_VER, +HW, 0, 0,
  M_COL, 1, 0.5f, 0,
  M_VER, 0, H, 0,

  M_VER, 0, H, 0,
  M_COL, 1, 1, 0,
  M_VER, -HW, 0, 0,
  M_VER, 0, 0, -L,

  M_VER, 0, 0, -L,
  M_VER, -HW, 0, 0,
  M_COL, 1, 0.5f, 0,
  M_VER, 0, H, 0,

  M_COL, 1, 1, 0,
  M_VER, -HW, 0, 0,
  M_VER, +HW, 0, 0,
  M_VER, 0, 0, L,

  M_COL, 0, 0, 0,
  M_VER, -HW*4/6, 0, 0,
  M_VER, -HW*3/6, -LH, 0,
  M_VER, -HW*2/6, 0, 0,
  M_VER, +HW*4/6, 0, 0,
  M_VER, +HW*3/6, -LH, 0,
  M_VER, +HW*2/6, 0, 0,

  M_VER, -HW*4/6, 0, L/3,
  M_VER, -HW*3/6, -LH, L/3,
  M_VER, -HW*2/6, 0, L/3,
  M_VER, +HW*4/6, 0, L/3,
  M_VER, +HW*3/6, -LH, L/3,
  M_VER, +HW*2/6, 0, L/3,

  M_VER, -HW*4/6, 0, L*2/3,
  M_VER, -HW*3/6, -LH, L*2/3,
  M_VER, -HW*2/6, 0, L*2/3,
  M_VER, +HW*4/6, 0, L*2/3,
  M_VER, +HW*3/6, -LH, L*2/3,
  M_VER, +HW*2/6, 0, L*2/3,

  M_END, 0, 0, 0,
};

static const Model fleaModel(fleaModelData, true);

Flea::Flea(Game& game)
: Enemy(game, fleaModel, 2),
  vy(0),
  vz(game.getSpeed() / 2.0f),
  blocked(false)
{ }

void Flea::update(float et) {
  vy += GRAVITY*et;
  // Move to new location.
  // See how far we moved in the Y direction; if its less than expected,
  // maybe jump.
  float newy = y + vy*et;
  blocked = !moveTo(x, newy, blocked? z : z + vz*et, blocked);
  if (blocked || newy != y)
    vy = 2.2f * (rand()/(float)RAND_MAX);
}

const float* Flea::getColour() const {
  static const float colour[] = { 1.0f, 1.0f, -0.3f };
  return colour;
}

float Flea::getPulseSpeed() const { return 22.0f; }
unsigned Flea::getAward() const { return 1000; }
const char* Flea::getDeathSound() const {
  return "sound/flea_death.pcm";
}
