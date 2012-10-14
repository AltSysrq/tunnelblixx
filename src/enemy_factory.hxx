#ifndef ENEMY_FACTORY_HXX_
#define ENEMY_FACTORY_HXX_

#include <list>

class Game;

class AbstractEnemyFactory;

class EnemyFactory {
  Game& game;
  std::list<AbstractEnemyFactory*> factories;

public:
  EnemyFactory(Game&);
  ~EnemyFactory();

  void update(float et, float speed);
};

#endif /* ENEMY_FACTORY_HXX_ */
