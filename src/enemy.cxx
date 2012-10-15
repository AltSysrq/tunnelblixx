#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cstdlib>
#include <algorithm>

#include "tunnel.hxx"
#include "enemy.hxx"
#include "game.hxx"
#include "player.hxx"
#include "mixer.hxx"
#include "sound_effect.hxx"

using namespace std;

Enemy::Enemy(Game& game_, const Model& model, unsigned hp)
: ModelledObject(&game_.field, 0, 0, -game_.getSpawnDistance(),
                 model, &game_.distortion),
  game(game_),
  hitPoints(hp)
{
  float x = rand() / (float)RAND_MAX;
  float px = game.getPlayerX();
  //TODO: Don't come within projectile radius of player

  moveTo(x, h/2, z, true);
}

void Enemy::collideWith(GameObject*) {}
void Enemy::collideWithPlayer(Player* p) {
  p->kill();
}

void Enemy::collideWithProjectile(Projectile*) {
  --hitPoints;
  if (hitPoints <= 0) {
    //Die
    alive = false;
    //Score
    //game.addScore(getAward());
    //Play sound
    SoundEffect::play(game.mixer, getDeathSound(),
                      (short)(0x7FFF / max(1.0f, -z/10.0f)));
    //Spawn confetti
    const float* colour = getColour();
    //field.add(new Confetti(field, x, y, z, colour[0], colour[1], colour[2],
    //                       &game.distortion));
    //Create pulses
    unsigned initialCol = (unsigned)(x / Tunnel::gsqsz);
    float speed = getPulseSpeed();
    //Reduce colour intensity since half is already in the base
    float r = colour[0]/2;
    float g = colour[1]/2;
    float b = colour[2]/2;

    game.tunnel.pulse(z, initialCol, r, g, b, -speed, 0);
    game.tunnel.pulse(z, initialCol, -r, -g, -b, speed, 0);
    for (unsigned i = 1; i <= initialCol; ++i) {
      game.tunnel.pulse(z, initialCol-i, r, g, b, -speed, i/speed);
      game.tunnel.pulse(z, initialCol-i, -r, -g, -b, speed, i/speed);
    }
    for (unsigned i = 1; initialCol+i < Tunnel::gridWidth; ++i) {
      game.tunnel.pulse(z, initialCol+i, r, g, b, -speed, i/speed);
      game.tunnel.pulse(z, initialCol+i, -r, -g, -b, speed, i/speed);
    }
  }
}
