#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>

#include "pillar.hxx"
#include "model.hxx"
#include "player.hxx"
#include "game.hxx"

#define HH 0.5f
#define R 0.1f

static const float pillarModelData[] = {
  M_COL, 1, 1, 1,

  M_VER, -R, -HH, -R,
  M_VER, -R, +HH, -R,
  M_VER, -R, +HH, +R,

  M_VER, -R, +HH, +R,
  M_VER, -R, -HH, -R,
  M_VER, -R, -HH, +R,

  M_VER, -R, -HH, -R,
  M_VER, -R, +HH, -R,
  M_VER, +R, +HH, -R,

  M_VER, +R, +HH, -R,
  M_VER, -R, -HH, -R,
  M_VER, +R, -HH, -R,

  M_VER, +R, -HH, -R,
  M_VER, +R, +HH, -R,
  M_VER, +R, +HH, +R,

  M_VER, +R, +HH, +R,
  M_VER, +R, -HH, -R,
  M_VER, +R, -HH, +R,

  M_VER, -R, -HH, +R,
  M_VER, -R, +HH, +R,
  M_VER, +R, +HH, +R,

  M_VER, +R, +HH, +R,
  M_VER, -R, -HH, +R,
  M_VER, +R, -HH, +R,

  M_END, 0, 0, 0,
};

static const Model pillarModel(pillarModelData, true);

Pillar::Pillar(Game& game)
: ModelledObject(&game.field, game.getPlayerX(), HH, -game.getSpawnDistance(),
                 pillarModel, &game.distortion)
{ }

void Pillar::update(float) {
  if (z > 0.25f) alive = false;
}

void Pillar::collideWith(GameObject*) {}
void Pillar::collideWithPlayer(Player* p) {
  p->kill();
}
