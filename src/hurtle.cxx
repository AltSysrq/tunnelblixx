#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "player.hxx"
#include "game.hxx"
#include "model.hxx"
#include "hurtle.hxx"

#define H 0.125f
#define T 0.1f

static const float hurtleModelData[] = {
  M_COL, 0, 0, 0,

  M_VER, 0*T, 0, 0,
  M_VER, 1*T, H, 0,
  M_VER, 2*T, 0, 0,

  M_VER, 2*T, 0, 0,
  M_VER, 3*T, H, 0,
  M_VER, 4*T, 0, 0,

  M_VER, 4*T, 0, 0,
  M_VER, 5*T, H, 0,
  M_VER, 6*T, 0, 0,

  M_VER, 6*T, 0, 0,
  M_VER, 7*T, H, 0,
  M_VER, 8*T, 0, 0,

  M_VER, 8*T, 0, 0,
  M_VER, 9*T, H, 0,
  M_VER, 10*T, 0, 0,

  M_COL, 1, 0, 0,
  M_VER, 0, H, 0,
  M_VER, 1, H, 0,
  M_VER, 0, H, -0.05f,

  M_VER, 1, H, 0,
  M_VER, 0, H, -0.05f,
  M_VER, 1, H, -0.05f,

  M_END, 0, 0, 0,
};

static const Model hurtleModel(hurtleModelData, true);

Hurtle::Hurtle(Game& game)
: ModelledObject(&game.field, 0.5f, H/2,
                 -game.getSpawnDistance(),
                 hurtleModel,
                 &game.distortion)
{ }

void Hurtle::update(float et) { }
void Hurtle::collideWith(GameObject*) { }
void Hurtle::collideWithPlayer(Player* p) {
  p->kill();
}
