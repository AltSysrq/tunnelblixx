#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <cstdlib>
#include <cmath>
#include <list>
#include <algorithm>

#include "enemy_factory.hxx"
#include "game.hxx"
#include "game_field.hxx"
#include "cactus.hxx"
#include "fire_fern.hxx"
#include "beetle.hxx"
#include "pillar.hxx"
#include "hurtle.hxx"

using namespace std;

#define PILLAR_INTERVAL 10.0f

class AbstractEnemyFactory {
public:
  virtual GameObject* update(float et, float speed, Game&) = 0;
  virtual ~AbstractEnemyFactory() {}
};


template<typename T>
class Factory: public AbstractEnemyFactory {
  const float baseSpawnInterval, minSpeed;
  float timeUntilSpawn;

public:
  Factory(float bsi, float ms)
  : baseSpawnInterval(bsi), minSpeed(ms), timeUntilSpawn(bsi)
  { }

  virtual GameObject* update(float et, float speed, Game& game) {
    GameObject* ret = NULL;
    if (speed > minSpeed) {
      timeUntilSpawn -= et*sqrt(speed-minSpeed);
      if (timeUntilSpawn < 0) {
        ret = new T(game);
        timeUntilSpawn = baseSpawnInterval * (0.2f + rand()/(float)RAND_MAX);
      }
    }

    return ret;
  }
};

EnemyFactory::EnemyFactory(Game& g)
: game(g), timeUntilPillar(PILLAR_INTERVAL)
{
  factories.push_back(new Factory<Cactus>(4.0f, 1.0f));
  factories.push_back(new Factory<FireFern>(5.0f, 3.0f));
  factories.push_back(new Factory<Beetle>(7.0f, 2.5f));
  factories.push_back(new Factory<Hurtle>(20.0f, 3.5f));
}

static void delete_it(AbstractEnemyFactory* it) { delete it; }
EnemyFactory::~EnemyFactory() {
  for_each(factories.begin(), factories.end(), delete_it);
}

void EnemyFactory::update(float et, float speed) {
  for (list<AbstractEnemyFactory*>::const_iterator it = factories.begin();
       it != factories.end(); ++it) {
    GameObject* go = (*it)->update(et, speed, game);
    if (go)
      game.field.add(go);
  }

  timeUntilPillar -= et;
  if (timeUntilPillar < 0) {
    timeUntilPillar += PILLAR_INTERVAL;

    game.field.add(new Pillar(game));
  }
}
