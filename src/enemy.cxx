#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cstdlib>
#include <algorithm>

#include "enemy.hxx"
#include "game.hxx"
#include "player.hxx"

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
