#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cmath>

#include "bat.hxx"
#include "game.hxx"
#include "player.hxx"
#include "model.hxx"

using namespace std;

#define GRAVITY -6.0f
#define XSPEED 0.4f
#define DROP_THRESH 4.0f

#define BHW 0.025f
#define BHH 0.025f
#define BHL 0.035f
#define HWS 0.325f

static const float flyingBatModelData[] = {
  M_COL, 1, 0, 1,
  M_VER, -BHW, -BHH, -BHL,
  M_VER, +BHW, -BHH, -BHL,
  M_VER, +BHW, +BHH, -BHL,

  M_VER, -BHW, -BHH, -BHL,
  M_VER, +BHW, +BHH, -BHL,
  M_VER, -BHW, +BHH, -BHL,

  M_VER, -BHW, -BHH, +BHL,
  M_VER, +BHW, -BHH, +BHL,
  M_VER, +BHW, +BHH, +BHL,

  M_VER, -BHW, -BHH, +BHL,
  M_VER, +BHW, +BHH, +BHL,
  M_VER, -BHW, +BHH, +BHL,

  M_VER, -BHW, -BHH, -BHL,
  M_VER, -BHW, +BHH, -BHL,
  M_VER, -BHW, +BHH, +BHL,

  M_VER, -BHW, -BHH, -BHL,
  M_VER, -BHW, +BHH, +BHL,
  M_VER, -BHW, -BHH, +BHL,

  M_VER, +BHW, -BHH, -BHL,
  M_VER, +BHW, +BHH, -BHL,
  M_VER, +BHW, +BHH, +BHL,

  M_VER, +BHW, -BHH, -BHL,
  M_VER, +BHW, +BHH, +BHL,
  M_VER, +BHW, -BHH, +BHL,

  M_VER, -BHW, -BHH, -BHL,
  M_VER, +BHW, -BHH, -BHL,
  M_VER, +BHW, -BHH, +BHL,

  M_VER, -BHW, -BHH, -BHL,
  M_VER, +BHW, -BHH, +BHL,
  M_VER, -BHL, -BHH, +BHL,

  M_VER, -BHW, +BHH, -BHL,
  M_VER, +BHW, +BHH, -BHL,
  M_VER, +BHW, +BHH, +BHL,

  M_VER, -BHW, +BHH, -BHL,
  M_VER, +BHW, +BHH, +BHL,
  M_VER, -BHL, +BHH, +BHL,

  M_VER, 0, -BHH, +BHL,
  M_VER, 0, +BHH, -BHL,
  M_COL, 0, 0, 0,
  M_VER, -HWS, 0, 1.2f*BHL,

  M_VER, +HWS, 0, 1.2f*BHL,
  M_COL, 1, 0, 1,
  M_VER, 0, +BHH, -BHL,
  M_VER, 0, -BHH, +BHL,

  M_END, 0, 0, 0,
};

static const Model flyingBatModel(flyingBatModelData, true);

#define FHH 0.1f
#define FHW 0.075f
static const float fallingBatModelData[] = {
  M_COL, 0.7f, 0, 0.7f,
  M_VER, 0, +FHH, 0,
  M_COL, 0, 0, 0,
  M_VER, -FHW, 0, -FHW,
  M_VER, +FHW, 0, -FHW,

  M_COL, 0.7f, 0, 0.7f,
  M_VER, 0, +FHH, 0,
  M_COL, 0, 0, 0,
  M_VER, -FHW, 0, -FHW,
  M_VER, -FHW, 0, +FHW,

  M_COL, 0.7f, 0, 0.7f,
  M_VER, 0, +FHH, 0,
  M_COL, 0, 0, 0,
  M_VER, +FHW, 0, -FHW,
  M_VER, +FHW, 0, +FHW,

  M_COL, 0.7f, 0, 0.7f,
  M_VER, 0, +FHH, 0,
  M_COL, 0, 0, 0,
  M_VER, -FHW, 0, +FHW,
  M_VER, +FHW, 0, +FHW,

  M_COL, 0.7f, 0, 0.7f,
  M_VER, 0, -FHH, 0,
  M_COL, 0, 0, 0,
  M_VER, -FHW, 0, -FHW,
  M_VER, +FHW, 0, -FHW,

  M_COL, 0.7f, 0, 0.7f,
  M_VER, 0, -FHH, 0,
  M_COL, 0, 0, 0,
  M_VER, -FHW, 0, -FHW,
  M_VER, -FHW, 0, +FHW,

  M_COL, 0.7f, 0, 0.7f,
  M_VER, 0, -FHH, 0,
  M_COL, 0, 0, 0,
  M_VER, +FHW, 0, -FHW,
  M_VER, +FHW, 0, +FHW,

  M_COL, 0.7f, 0, 0.7f,
  M_VER, 0, -FHH, 0,
  M_COL, 0, 0, 0,
  M_VER, -FHW, 0, +FHW,
  M_VER, +FHW, 0, +FHW,

  M_END, 0, 0, 0,
};

static const Model fallingBatModel(fallingBatModelData, true);

Bat::Bat(Game& game_)
: Enemy(game_, flyingBatModel, 1),
  isFlying(true),
  game(game_),
  vy(0), vz(game_.getSpeed()/2.0f)
{
  y = 0.4f + h/2;
}

void Bat::update(float et) {
  if (isFlying) {
    // Fall to the ground if close enough to the player
    if (-z < DROP_THRESH) {
      isFlying = false;
      setModel(fallingBatModel);
    } else {
      // Stay in the air and follow the player
      float newx;
      if (fabs(x - game.getPlayerX() < XSPEED*et))
        newx = game.getPlayerX();
      else if (x < game.getPlayerX())
        newx = x + XSPEED*et;
      else
        newx = x - XSPEED*et;

      moveTo(newx, y, z+vz*et, true);
    }
  } else {
    // Fall to the ground
    vy += GRAVITY*et;
    moveTo(x, y + vy*et, z, true);
  }
}

void Bat::collideWithProjectile(Projectile* p) {
  /* No effect unless flying */
  if (isFlying)
    Enemy::collideWithProjectile(p);
}

const float* Bat::getColour() const {
  static const float colour[] = { 1.0f, -1.0f, 1.0f };
  return colour;
}

float Bat::getPulseSpeed() const { return 20.0f; }
unsigned Bat::getAward() const { return 750; }
const char* Bat::getDeathSound() const {
  return "sound/bat_death.pcm";
}
