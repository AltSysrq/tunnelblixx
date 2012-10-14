#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <list>
#include <algorithm>

#include "game_field.hxx"
#include "game_object.hxx"

using namespace std;

static void delete_it(GameObject* it) {
  delete it;
}

GameField::~GameField() {
  for_each(objects.begin(), objects.end(), delete_it);
  for_each(toSpawn.begin(), toSpawn.end(), delete_it);
}

void GameField::update(float et) {
  //Update all objects
  for (list<GameObject*>::iterator it = objects.begin();
       it != objects.end(); ++it) {
    (*it)->update(et);
    //Delete and remove if dead
    if (!(*it)->isAlive()) {
      delete *it;
      it = objects.erase(it);
    }
  }

  //Add newly-spawned objects
  objects.insert(objects.end(), toSpawn.begin(), toSpawn.end());
  toSpawn.clear();
}

void GameField::draw() const {
  for (iterator it = begin(); it != end(); ++it)
    (*it)->draw();
}

void GameField::add(GameObject* go) {
  toSpawn.push_back(go);
}

GameField::iterator GameField::begin() const {
  return objects.begin();
}

GameField::iterator GameField::end() const {
  return objects.end();
}
